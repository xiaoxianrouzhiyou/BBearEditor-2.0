#include "BBFLCGlobalIllumination.h"
#include "BBGlobalIllumination.h"
#include "Scene/BBScene.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBMaterial.h"
#include "Render/BBRenderPass.h"
#include "3D/BBModel.h"
#include "Render/BufferObject/BBShaderStorageBufferObject.h"
#include "Render/BufferObject/BBAtomicCounterBufferObject.h"
#include "Math/BBMath.h"


float* BBFLCGlobalIllumination::m_S = nullptr;
float* BBFLCGlobalIllumination::m_Sp = nullptr;
float* BBFLCGlobalIllumination::m_Noise = nullptr;
int BBFLCGlobalIllumination::m_nNoiseCount = 64;
int BBFLCGlobalIllumination::m_SNum = 5;
BBShaderStorageBufferObject* BBFLCGlobalIllumination::m_pTriangleCutSSBOSet = nullptr;
int BBFLCGlobalIllumination::m_nSSBOCount = 0;
BBAtomicCounterBufferObject* BBFLCGlobalIllumination::m_pTriangleIdACBO = nullptr;


void BBFLCGlobalIllumination::open(BBScene *pScene)
{
    if (!m_S)
    {
        m_S = generateS();
        m_Sp = generateSp(m_S);
        m_Noise = generateNoise();
    }

    clear(pScene);

    // used for the id of triangle cut
    m_pTriangleIdACBO = new BBAtomicCounterBufferObject();

    BBGlobalIllumination::setGBufferPassByUsingExtraMaterial(pScene);
    // Divide triangles evenly
    setTriangleCutPass(pScene);
    setIndirectShadingPass(pScene);
}

void BBFLCGlobalIllumination::setTriangleCutPass(BBScene *pScene)
{
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_FLC_TriangleCut",
                    BB_PATH_RESOURCE_SHADER(GI/FLC_TriangleCut_VS.vert),
                    BB_PATH_RESOURCE_SHADER(GI/FLC_TriangleCut_FS.frag),
                    BB_PATH_RESOURCE_SHADER(GI/FLC_TriangleCut_GS.shader));

    // test
    float *pLightPosition = new float[4] {1.0f, 1.0f, 0.0f, 0.0f};
    float *pLightColor = new float[4] {1.0f, 1.0f, 1.0f, 1.0f};
    pMaterial->setVector4(LOCATION_LIGHT_POSITION, pLightPosition);
    pMaterial->setVector4(LOCATION_LIGHT_COLOR, pLightColor);

    QList<BBGameObject*> objects = pScene->getModels();
    m_nSSBOCount = objects.count();
    m_pTriangleCutSSBOSet = new BBShaderStorageBufferObject[m_nSSBOCount];

    for (int i = 0; i < m_nSSBOCount; i++)
    {
        BBModel *pModel = (BBModel*)objects[i];
        // 0 is used by GBuffer
        pModel->setExtraMaterial(1, pMaterial);

        BBMesh *pMesh = pModel->getMesh();

        pMesh->appendACBO(m_pTriangleIdACBO, true);

        // Send SSBO to write the result of triangle cut
        // "1" is consistent with the "binding" in shader
        // struct TriangleCut
        // {
        //     vec4 center;
        //     vec4 normal_and_level;
        //     vec4 color_and_area;
        // };
        // 3x4 float
        m_pTriangleCutSSBOSet[i].createBufferObject<float>(1, 300000 * m_SNum * 12, GL_STATIC_DRAW, nullptr);
        m_pTriangleCutSSBOSet[i].submitData();
        pMesh->appendSSBO(&m_pTriangleCutSSBOSet[i]);
    }
}

void BBFLCGlobalIllumination::setIndirectShadingPass(BBScene *pScene)
{
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(0);
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_FLC_IndirectShading", BB_PATH_RESOURCE_SHADER(GI/FullScreenQuad.vert), BB_PATH_RESOURCE_SHADER(GI/IndirectShading.frag));

    pMaterial->setSampler2D("AlbedoTex", pScene->getColorFBO(0, 0));
    pMaterial->setSampler2D("NormalTex", pScene->getColorFBO(0, 1));
    pMaterial->setSampler2D("PositionTex", pScene->getColorFBO(0, 2));

    pFullScreenQuad->setCurrentMaterial(pMaterial);

    // Only one model is considered for the time being
    // do not clear ACBO, use data calculated in the TriangleCutPass
    pFullScreenQuad->appendACBO(m_pTriangleIdACBO, false);
    pFullScreenQuad->appendSSBO(&m_pTriangleCutSSBOSet[0]);
}

void BBFLCGlobalIllumination::clear(BBScene *pScene)
{
    // remove TriangleCutSSBO that is setted last time
    QList<BBGameObject*> objects = pScene->getModels();
    int nCount = objects.count();
    for (int i = 0; i < nCount; i++)
    {
        BBModel *pModel = (BBModel*)objects[i];
        BBMesh *pMesh = pModel->getMesh();

        pMesh->removeACBO();
        pMesh->removeSSBO(&m_pTriangleCutSSBOSet[i]);
    }

    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(0);
    pFullScreenQuad->removeACBO();
    pFullScreenQuad->removeSSBO(&m_pTriangleCutSSBOSet[0]);

    BB_SAFE_DELETE_ARRAY(m_pTriangleCutSSBOSet);
    BB_SAFE_DELETE(m_pTriangleIdACBO);
}

float* BBFLCGlobalIllumination::generateS()
{
    // Scene radius
    static float R = 1;
    // The average number of virtual point lights near the current fragment
    // This can not be calculated directly, the author estimated the value
    static float D = 0.2 * R;
    // Value between 64-1024 (the larger it is, the smaller the triangle is)
    static float N = 64;
    // It is a number greater than 1
    static float u = 1.2;

    float *pS = new float[m_SNum];
    pS[0] = 4 * PI * D * D / N;
    float ui = u;
    for (int i = 1; i < m_SNum; i++)
    {
        pS[i] = pS[0] * ui;
        ui *= u;
    }

    return pS;
}

float* BBFLCGlobalIllumination::generateSp(float *pS)
{
    float *pSp = new float[m_SNum];
    pSp[0] = pS[0];
    for (int i = 1; i < m_SNum; i++)
    {
        float sum = 0;
        for (int j = 0; j < i; j++)
        {
            sum += 1.0 / pS[j];
        }
        pSp[i] = 1.0 / sum;
    }
    return pSp;
}

float* BBFLCGlobalIllumination::generateNoise()
{
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
    std::default_random_engine generator;
    float *pNoise = new float[m_nNoiseCount];
    for (int i = 0; i < m_nNoiseCount; i++)
    {
        pNoise[i] = randomFloats(generator);
    }
    return pNoise;
}
