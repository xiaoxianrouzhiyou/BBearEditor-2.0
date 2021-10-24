#include "BBSPHFluidRenderer.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBDrawCall.h"
#include "Render/Texture/BBProcedureTexture.h"
#include "Render/BBRenderPass.h"
#include "BBSPHParticleSystem.h"
#include "Scene/BBScene.h"
#include "Scene/BBSceneManager.h"
#include "2D/BBFullScreenQuad.h"


BBSPHFluidRenderer::BBSPHFluidRenderer(const QVector3D &position)
    : BBRenderableObject(position, QVector3D(0, 0, 0), QVector3D(1, 1, 1)),
      m_nSSFGBufferMaterialIndex(3)
{
    m_pScreenQuadXFilterMaterial = nullptr;
    m_pScreenQuadYFilterMaterial = nullptr;
    m_pScreenQuadNormalMaterial = nullptr;
    m_pScreenQuadShadingMaterial = nullptr;
}

BBSPHFluidRenderer::~BBSPHFluidRenderer()
{
    BB_SAFE_DELETE(m_pScreenQuadXFilterMaterial);
    BB_SAFE_DELETE(m_pScreenQuadYFilterMaterial);
    BB_SAFE_DELETE(m_pScreenQuadNormalMaterial);
    BB_SAFE_DELETE(m_pScreenQuadShadingMaterial);
}

void BBSPHFluidRenderer::init(BBSPHParticleSystem *pParticleSystem)
{   
    m_pParticleSystem = pParticleSystem;

    m_pVBO = new BBVertexBufferObject(pParticleSystem->getSize());
    for (int i = 0; i < m_pVBO->getVertexCount(); i++)
    {
        m_pVBO->setPosition(i, pParticleSystem->getParticle(i)->m_Position);
        m_pVBO->setColor(i, BBConstant::m_LightGreen);
    }

    m_pCurrentMaterial->init("BBSPHFluidParticle",
                             BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SPHFluidParticle.vert),
                             BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SPHFluidParticle.frag));


    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setSSBO(m_pSSBO);
    pDrawCall->setVBO(m_pVBO, GL_POINTS, 0, m_pVBO->getVertexCount());
    appendDrawCall(pDrawCall);


    // SSF
    initSSFMaterial();
}

void BBSPHFluidRenderer::render(BBCamera *pCamera)
{
    for (int i = 0; i < m_pVBO->getVertexCount(); i++)
    {
        m_pVBO->setPosition(i, m_pParticleSystem->getParticle(i)->m_Position);
    }
    m_pVBO->submitData();

    BBRenderableObject::render(pCamera);
}

void BBSPHFluidRenderer::switchSSF(bool bEnable)
{
    BBScene *pScene = BBSceneManager::getScene();
    if (bEnable)
    {
        pScene->setRenderingFunc(&BBScene::deferredRendering1_3);

        setCurrentMaterial(getExtraMaterial(m_nSSFGBufferMaterialIndex));

        BBFullScreenQuad *pFullScreenQuad0 = pScene->getFullScreenQuad(0);
        pFullScreenQuad0->setCurrentMaterial(m_pScreenQuadXFilterMaterial);

        BBFullScreenQuad *pFullScreenQuad1 = pScene->getFullScreenQuad(1);
        pFullScreenQuad1->setCurrentMaterial(m_pScreenQuadYFilterMaterial);

        BBFullScreenQuad *pFullScreenQuad2 = pScene->getFullScreenQuad(2);
        pFullScreenQuad2->setCurrentMaterial(m_pScreenQuadNormalMaterial);
    }
    else
    {
        pScene->setRenderingFunc(&BBScene::defaultRendering);

        restoreMaterial();
    }
}

void BBSPHFluidRenderer::initSSFMaterial()
{
    BBScene *pScene = BBSceneManager::getScene();

    // GBuffer pass
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("SSF_1_GBuffer",
                    BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SSF_VS_GBuffer.vert),
                    BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SSF_FS_1_GBuffer.frag),
                    BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SSF_GS_GBuffer.shader));
    pMaterial->setFloat("ParticleRadius", 0.125f);

    pMaterial->setBlendState(true);
    pMaterial->setBlendFunc(GL_ONE, GL_ONE);
    pMaterial->setZTestState(false);

    setExtraMaterial(m_nSSFGBufferMaterialIndex, pMaterial);

    // x Filter pass
    m_pScreenQuadXFilterMaterial = new BBMaterial();
    m_pScreenQuadXFilterMaterial->init("SSF_2_Screen_Filter",
                                       BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/FullScreenQuad.vert),
                                       BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SSF_FS_2_Screen_Filter.frag));
    m_pScreenQuadXFilterMaterial->setSampler2D("DepthMap", pScene->getColorFBO(0, 0));
    m_pScreenQuadXFilterMaterial->setSampler2D("ThicknessMap", pScene->getColorFBO(0, 1));
    m_pScreenQuadXFilterMaterial->setFloat("FilterRadius", 3.0f);
    m_pScreenQuadXFilterMaterial->setFloat("SpatialScale", 0.2f);
    m_pScreenQuadXFilterMaterial->setFloat("RangeScale", 0.2f);
    m_pScreenQuadXFilterMaterial->setVector4("BlurDir", 1.0f, 0.0f, 0.0f, 0.0f);

    // y Filter pass
    m_pScreenQuadYFilterMaterial = new BBMaterial();
    m_pScreenQuadYFilterMaterial->init("SSF_2_Screen_Filter",
                                       BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/FullScreenQuad.vert),
                                       BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SSF_FS_2_Screen_Filter.frag));
    m_pScreenQuadYFilterMaterial->setSampler2D("DepthMap", pScene->getColorFBO(1, 0));
    m_pScreenQuadYFilterMaterial->setSampler2D("ThicknessMap", pScene->getColorFBO(1, 1));
    m_pScreenQuadYFilterMaterial->setFloat("FilterRadius", 3.0f);
    m_pScreenQuadYFilterMaterial->setFloat("SpatialScale", 0.2f);
    m_pScreenQuadYFilterMaterial->setFloat("RangeScale", 0.2f);
    m_pScreenQuadYFilterMaterial->setVector4("BlurDir", 0.0f, 1.0f, 0.0f, 0.0f);

    // Compute normals pass
    m_pScreenQuadNormalMaterial = new BBMaterial();
    m_pScreenQuadNormalMaterial->init("SSF_3_Screen_Normal",
                                      BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/FullScreenQuad.vert),
                                      BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SSF_FS_3_Screen_Normal.frag));
    m_pScreenQuadNormalMaterial->setSampler2D("DepthMap", pScene->getColorFBO(2, 0));
}
