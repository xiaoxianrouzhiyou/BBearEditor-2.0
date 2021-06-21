#include "BBDrawCall.h"
#include "BBVertexBufferObject.h"
#include "BBElementBufferObject.h"
#include "BBCamera.h"
#include "BBRenderPass.h"


BBDrawCall::BBDrawCall()
{
    m_pMaterial = nullptr;

    m_eDrawPrimitiveType = GL_TRIANGLES;

    m_pVBO = nullptr;
    m_nDrawStartIndex = 0;
    m_nDrawCount = 3;

    m_pEBO = nullptr;
    m_nIndexCount = 0;
}

void BBDrawCall::setMaterial(BBMaterial *pMaterial)
{
    m_pMaterial = pMaterial;
}

void BBDrawCall::setVBO(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType, int nDrawStartIndex, int nDrawCount)
{
    m_pVBO = pVBO;
    m_eDrawPrimitiveType = eDrawPrimitiveType;
    m_nDrawStartIndex = nDrawStartIndex;
    m_nDrawCount = nDrawCount;
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
        m_pVBO->draw(m_eDrawPrimitiveType, m_nDrawStartIndex, m_nDrawCount);
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
