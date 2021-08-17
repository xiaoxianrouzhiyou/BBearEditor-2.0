#include "BBParticle.h"
#include "Render/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/Texture/BBProcedureTexture.h"
#include "Render/BBRenderPass.h"
#include "Render/BBDrawCall.h"


BBParticle::BBParticle(const QVector3D &position)
    : BBRenderableObject(position, QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{

}

void BBParticle::init()
{
    create0();

    m_pCurrentMaterial->getBaseRenderPass()->setBlendState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_pCurrentMaterial->getBaseRenderPass()->setZTestState(false);
    m_pCurrentMaterial->getBaseRenderPass()->setPointSpriteState(true);
    m_pCurrentMaterial->getBaseRenderPass()->setProgramPointSizeState(true);

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO, GL_POINTS, 0, m_pVBO->getVertexCount());
    appendDrawCall(pDrawCall);
}

void BBParticle::render(BBCamera *pCamera)
{
    update0();
    m_pVBO->submitData();
    BBRenderableObject::render(pCamera);
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
    m_pCurrentMaterial->getBaseRenderPass()->setSampler2D(LOCATION_TEXTURE(0), texture.create(128));
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
}
