#include "BBDrawCall.h"
#include "BBVertexBufferObject.h"
#include "BBElementBufferObject.h"
#include "BBCamera.h"
#include "BBRenderPass.h"
#include "Scene/BBSceneManager.h"
#include "Scene/BBScene.h"
#include "Render/Light/BBLight.h"
#include "Render/BBFrameBufferObject.h"


BBDrawFunc BBDrawCall::m_DrawFunc = &BBDrawCall::forwardRendering;

BBDrawCall::BBDrawCall()
    : BBBaseRenderComponent()
{
    m_pMaterial = nullptr;

    m_eDrawPrimitiveType = GL_TRIANGLES;
    m_nDrawStartIndex = 0;

    m_pVBO = nullptr;
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

void BBDrawCall::setEBO(BBElementBufferObject *pEBO, GLenum eDrawPrimitiveType, int nIndexCount, int nDrawStartIndex)
{
    m_pEBO = pEBO;
    m_eDrawPrimitiveType = eDrawPrimitiveType;
    m_nIndexCount = nIndexCount;
    m_nDrawStartIndex = nDrawStartIndex;
}

void BBDrawCall::draw(BBCamera *pCamera)
{
    (this->*m_DrawFunc)(pCamera);
}

void BBDrawCall::setDrawFunc(int nIndex)
{
    switch (nIndex) {
    case 0:
        m_DrawFunc = &BBDrawCall::forwardRendering;
        BBSceneManager::getScene()->enableFBO(false);
        break;
    case 1:
        m_DrawFunc = &BBDrawCall::deferredRendering;
        BBSceneManager::getScene()->enableFBO(true);
        break;
    default:
        break;
    }
}

void BBDrawCall::onePassRendering(BBCamera *pCamera)
{
    m_pVBO->bind();
    m_pMaterial->getBaseRenderPass()->bind(pCamera);
    if (m_pEBO == nullptr)
    {
        m_pVBO->draw(m_eDrawPrimitiveType, m_nDrawStartIndex, m_nDrawCount);
    }
    else
    {
        m_pEBO->bind();
        m_pEBO->draw(m_eDrawPrimitiveType, m_nIndexCount, m_nDrawStartIndex);
        m_pEBO->unbind();
    }
    m_pMaterial->getBaseRenderPass()->unbind();
    m_pVBO->unbind();
}

void BBDrawCall::forwardRendering(BBCamera *pCamera)
{
    QList<BBGameObject*> lights = collectLights();

    m_pVBO->bind();

    // base
    if (lights.count() > 0)
    {
        // render the first light
        ((BBLight*)lights[0])->setRenderPass(m_pMaterial->getBaseRenderPass());
    }
    m_pMaterial->getBaseRenderPass()->bind(pCamera);
    if (m_pEBO == nullptr)
    {
        m_pVBO->draw(m_eDrawPrimitiveType, m_nDrawStartIndex, m_nDrawCount);
    }
    else
    {
        m_pEBO->bind();
        m_pEBO->draw(m_eDrawPrimitiveType, m_nIndexCount, m_nDrawStartIndex);
        m_pEBO->unbind();
    }
    m_pMaterial->getBaseRenderPass()->unbind();

    // additive
    BBRenderPass *pAdditiveRenderPass = m_pMaterial->getAdditiveRenderPass();
    if (lights.count() > 1 && pAdditiveRenderPass)
    {
        for (int i = 1; i < lights.count(); i++)
        {
            ((BBLight*)lights[i])->setRenderPass(pAdditiveRenderPass);
            pAdditiveRenderPass->bind(pCamera);
            if (m_pEBO == nullptr)
            {
                m_pVBO->draw(m_eDrawPrimitiveType, m_nDrawStartIndex, m_nDrawCount);
            }
            else
            {
                m_pEBO->bind();
                m_pEBO->draw(m_eDrawPrimitiveType, m_nIndexCount, m_nDrawStartIndex);
                m_pEBO->unbind();
            }
        }
        pAdditiveRenderPass->unbind();
    }

    m_pVBO->unbind();

    if (m_pNext != nullptr)
    {
        next<BBDrawCall>()->draw(pCamera);
    }
}

void BBDrawCall::deferredRendering(BBCamera *pCamera)
{
    m_pVBO->bind();
    m_pMaterial->getBaseRenderPass()->bind(pCamera);
    if (m_pEBO == nullptr)
    {
        m_pVBO->draw(m_eDrawPrimitiveType, m_nDrawStartIndex, m_nDrawCount);
    }
    else
    {
        m_pEBO->bind();
        m_pEBO->draw(m_eDrawPrimitiveType, m_nIndexCount, m_nDrawStartIndex);
        m_pEBO->unbind();
    }
    m_pMaterial->getBaseRenderPass()->unbind();
    m_pVBO->unbind();
}

QList<BBGameObject*> BBDrawCall::collectLights()
{
    return BBSceneManager::getScene()->getLights();
}
