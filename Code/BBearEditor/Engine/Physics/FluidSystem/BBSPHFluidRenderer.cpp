#include "BBSPHFluidRenderer.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBDrawCall.h"
#include "Render/Texture/BBProcedureTexture.h"
#include "Render/BBRenderPass.h"
#include "BBSPHParticleSystem.h"


BBSPHFluidRenderer::BBSPHFluidRenderer(const QVector3D &position)
    : BBRenderableObject(position, QVector3D(0, 0, 0), QVector3D(1, 1, 1))
{

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

//    m_pCurrentMaterial->init("BBSPHFluidParticle",
//                             BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SPHFluidParticle.vert),
//                             BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SPHFluidParticle.frag));

    m_pCurrentMaterial->init("BBSPHFluidParticle",
                             BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SPHFluid_SSF_VS.vert),
                             BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SPHFluid_SSF_FS_Depth.frag),
                             BB_PATH_RESOURCE_SHADER(Physics/FluidSystem/SPHFluid_SSF_GS.shader));
    m_pCurrentMaterial->setFloat("ParticleRadius", 0.125f);

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setSSBO(m_pSSBO);
    pDrawCall->setVBO(m_pVBO, GL_POINTS, 0, m_pVBO->getVertexCount());
    appendDrawCall(pDrawCall);
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
