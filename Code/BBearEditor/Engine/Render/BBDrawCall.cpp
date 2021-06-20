#include "BBDrawCall.h"
#include "BBVertexBufferObject.h"
#include "BBElementBufferObject.h"
#include "BBCamera.h"
#include "BBRenderPass.h"


BBDrawCall::BBDrawCall()
{
    m_pMaterial = nullptr;
    m_pVBO = nullptr;
    m_pEBO = nullptr;
}

void BBDrawCall::setMaterial(BBMaterial *pMaterial)
{
    m_pMaterial = pMaterial;
}

void BBDrawCall::setVBO(BBVertexBufferObject *pVBO)
{
    m_pVBO = pVBO;
}

void BBDrawCall::setEBO(BBElementBufferObject *pEBO)
{
    m_pEBO = pEBO;
}

void BBDrawCall::draw(BBCamera *pCamera)
{
    m_pVBO->bind();

    m_pMaterial->getBaseRenderPass()->bind(pCamera);
    if (m_pEBO == nullptr)
    {
        m_pVBO->draw();
    }
//    m_pMaterial->getAdditiveRenderPass()->bind(pCamera);
//    if (m_pEBO == nullptr)
//    {
//        m_pVBO->draw();
//    }

    m_pVBO->unbind();

    if (m_pNext != nullptr)
    {
        next<BBDrawCall>()->draw(pCamera);
    }
}
