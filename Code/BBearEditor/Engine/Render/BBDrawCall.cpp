#include "BBDrawCall.h"
#include "BBVertexBufferObject.h"
#include "BBElementBufferObject.h"
#include "BBCamera.h"


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
    m_pMaterial->bind(pCamera);

    if (m_pEBO == nullptr)
    {
        glDrawArrays(GL_TRIANGLES, 0, m_pVBO->getVertexCount());
    }

    // test
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_pVBO->unbind();

    if (m_pNext != nullptr)
    {
        next<BBDrawCall>()->draw(pCamera);
    }
}
