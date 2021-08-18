#include "BBParticle.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BufferObject/BBShaderStorageBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/Texture/BBProcedureTexture.h"
#include "Render/BBRenderPass.h"
#include "Render/BBDrawCall.h"
#include "Math/BBMath.h"
#include "Render/BBShader.h"


BBParticle::BBParticle(const QVector3D &position)
    : BBRenderableObject(position, QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{
    m_pSSBO = nullptr;
    m_nVertexCount = 1 << 15;
    m_pUpdateCShader = nullptr;
}

BBParticle::~BBParticle()
{
    BB_SAFE_DELETE(m_pSSBO);
    BB_SAFE_DELETE(m_pUpdateCShader);
}

void BBParticle::init()
{
    create1();

    m_pCurrentMaterial->getBaseRenderPass()->setBlendState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_pCurrentMaterial->getBaseRenderPass()->setZTestState(false);
    m_pCurrentMaterial->getBaseRenderPass()->setPointSpriteState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setProgramPointSizeState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setCullState(false);
}

void BBParticle::render(BBCamera *pCamera)
{
    update1();
    m_pDrawCalls->renderOnePassSSBO(pCamera);
}

void BBParticle::create0()
{
    m_pVBO = new BBVertexBufferObject(180);
    for (int i = 0; i < 180; i++)
    {
        m_pVBO->setPosition(i, 2 * cosf(0.139626f * i), 0.0f, 2 * sinf(0.139626f * i));
        m_pVBO->setColor(i, 0.1f, 0.4f, 0.6f);
    }
    m_pCurrentMaterial->init("Particles0", BB_PATH_RESOURCE_SHADER(ParticleSystem/Particles0.vert), BB_PATH_RESOURCE_SHADER(ParticleSystem/Particles0.frag));

    BBProcedureTexture texture;
    m_pCurrentMaterial->getBaseRenderPass()->setSampler2D(LOCATION_TEXTURE(0), texture.create0(128));

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setSSBO(m_pSSBO);
    pDrawCall->setVBO(m_pVBO, GL_POINTS, 0, m_pVBO->getVertexCount());
    appendDrawCall(pDrawCall);
}

void BBParticle::update0()
{
    setRotation(10, QVector3D(0, 1, 0));
    for (int i = 0; i < m_pVBO->getVertexCount(); i++)
    {
        m_pVBO->setNormal(i, 0, 0.1f * i, 0);
        if (i > 90)
            break;
    }
    for (int i = 0; i < m_pVBO->getVertexCount(); i++)
    {
        float r = 0.02f * i;
        if (r > 1.0f)
            r -= 1.0f;
        m_pVBO->setColor(i, r, 0.4f, 0.6f);
    }
    m_pVBO->submitData();
}

void BBParticle::create1()
{
    m_pSSBO = new BBShaderStorageBufferObject(m_nVertexCount);
    m_nIndexCount = 6 * m_nVertexCount;
    m_pIndexes = new unsigned short[m_nIndexCount];
    unsigned short *pCurrent = m_pIndexes;
    for (int i = 0; i < m_nVertexCount; i++)
    {
        m_pSSBO->setPosition(i, sfrandom(), sfrandom(), sfrandom());
        m_pSSBO->setColor(i, 0.1f, 0.4f, 0.6f);
        // used as the speed of update
        m_pSSBO->setNormal(i, 0.0f, 0.0f, 0.0f);

        unsigned short index(i << 2);

        *(pCurrent++) = index;
        *(pCurrent++) = index + 1;
        *(pCurrent++) = index + 2;

        *(pCurrent++) = index;
        *(pCurrent++) = index + 2;
        *(pCurrent++) = index + 3;
    }

    m_pCurrentMaterial->init("PointSpriteSSBO",
                             BB_PATH_RESOURCE_SHADER(ParticleSystem/PointSpriteSSBO.vert),
                             BB_PATH_RESOURCE_SHADER(ParticleSystem/PointSpriteSSBO.frag));

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setSSBO(m_pSSBO);
    pDrawCall->setEBO(m_pEBO, GL_TRIANGLES, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);

    // compute shader of update
    m_pUpdateCShader = new BBShader();
    m_pUpdateCShader->init(BB_PATH_RESOURCE_SHADER(ParticleSystem/UpdateParticles.shader));
}

void BBParticle::update1()
{
    m_pSSBO->submitData();
}
