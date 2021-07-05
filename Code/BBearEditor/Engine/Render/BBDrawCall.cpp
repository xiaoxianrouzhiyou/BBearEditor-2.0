#include "BBDrawCall.h"
#include "BBVertexBufferObject.h"
#include "BBElementBufferObject.h"
#include "BBCamera.h"
#include "BBRenderPass.h"
#include "Scene/BBSceneManager.h"
#include "Scene/BBScene.h"
#include "Render/Light/BBLight.h"
#include "Render/BBFrameBufferObject.h"
#include "Base/BBRenderableObject.h"


/**
 * @brief BBDrawCall::BBDrawCall
 */
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

    if (m_pNext != nullptr)
    {
        next<BBDrawCall>()->setMaterial(pMaterial);
    }
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

float BBDrawCall::getDistanceToCamera(BBCamera *pCamera)
{
    return m_pRenderableObject->getPosition().distanceToPoint(pCamera->getPosition());
}

void BBDrawCall::draw(BBCamera *pCamera)
{
    (this->*m_DrawFunc)(pCamera);
}

void BBDrawCall::switchRenderingSettings(int nIndex)
{
    switch (nIndex) {
    case 0:
        m_DrawFunc = &BBDrawCall::forwardRendering;
        BBSceneManager::enableDeferredRendering(false);
        break;
    case 1:
        m_DrawFunc = &BBDrawCall::deferredRendering;
        BBSceneManager::enableDeferredRendering(true);
        break;
    default:
        break;
    }
}

void BBDrawCall::onePassRendering(BBCamera *pCamera)
{
    QList<BBGameObject*> lights = collectLights();
    onePassRendering(pCamera, lights);
}

void BBDrawCall::onePassRendering(BBCamera *pCamera, QList<BBGameObject*> lights)
{
    m_pVBO->bind();
    BBRenderPass *pBaseRenderPass = m_pMaterial->getBaseRenderPass();
    for (int i = 0; i < lights.count(); i++)
    {
        BBLight *pLight = (BBLight*)lights[i];
        pLight->setRenderPass(pBaseRenderPass);
        pBaseRenderPass->bind(pCamera);
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
    pBaseRenderPass->unbind();
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
        BBLight *pLight = (BBLight*)lights[0];
        pLight->setRenderPass(m_pMaterial->getBaseRenderPass());
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
            BBLight *pLight = (BBLight*)lights[i];
            pLight->setRenderPass(pAdditiveRenderPass);
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


/**
 * @brief BBRenderQueue::BBRenderQueue
 */
BBRenderQueue::BBRenderQueue(BBCamera *pCamera)
{
    m_pCamera = pCamera;
    m_pOpaqueDrawCall = nullptr;
    m_pTransparentDrawCall = nullptr;
    m_pUIDrawCall = nullptr;
}

BBRenderQueue::~BBRenderQueue()
{
    BB_SAFE_DELETE(m_pOpaqueDrawCall);
    BB_SAFE_DELETE(m_pTransparentDrawCall);
    BB_SAFE_DELETE(m_pUIDrawCall);
}

void BBRenderQueue::appendOpaqueDrawCall(BBDrawCall *pDC)
{
    if (m_pOpaqueDrawCall == nullptr)
    {
        m_pOpaqueDrawCall = pDC;
    }
    else
    {
        appendAscendingRenderQueue(m_pOpaqueDrawCall, pDC);
    }
}

void BBRenderQueue::appendTransparentDrawCall(BBDrawCall *pDC)
{
    if (m_pTransparentDrawCall == nullptr)
    {
        m_pTransparentDrawCall = pDC;
    }
    else
    {
        appendAscendingRenderQueue(m_pTransparentDrawCall, pDC);
    }
}

void BBRenderQueue::appendUIDrawCall(BBDrawCall *pDC)
{

}

void BBRenderQueue::removeOpaqueDrawCall(BBDrawCall *pDC)
{
    BB_PROCESS_ERROR_RETURN(m_pOpaqueDrawCall);
    if (m_pOpaqueDrawCall->isEnd())
    {
        m_pOpaqueDrawCall = nullptr;
    }
    else
    {
        m_pOpaqueDrawCall->remove(pDC);
    }
}

void BBRenderQueue::removeTransparentDrawCall(BBDrawCall *pDC)
{
    BB_PROCESS_ERROR_RETURN(m_pTransparentDrawCall);
    if (m_pTransparentDrawCall->isEnd())
    {
        m_pTransparentDrawCall = nullptr;
    }
    else
    {
        m_pTransparentDrawCall->remove(pDC);
    }
}

void BBRenderQueue::removeUIDrawCall(BBDrawCall *pDC)
{

}

void BBRenderQueue::render()
{
    renderOpaque();
    renderTransparent();
    renderUI();
}

void BBRenderQueue::renderOpaque()
{
    BB_PROCESS_ERROR_RETURN(m_pOpaqueDrawCall);
    m_pOpaqueDrawCall->draw(m_pCamera);
}

void BBRenderQueue::renderTransparent()
{
    BB_PROCESS_ERROR_RETURN(m_pTransparentDrawCall);
    m_pTransparentDrawCall->draw(m_pCamera);
}

void BBRenderQueue::renderUI()
{
    BB_PROCESS_ERROR_RETURN(m_pUIDrawCall);
    m_pUIDrawCall->draw(m_pCamera);
}

void BBRenderQueue::appendAscendingRenderQueue(BBDrawCall *pHead, BBDrawCall *pNewNode)
{
    BBDrawCall *pCurrent = pHead;
    BBDrawCall *pPrevious = pCurrent;
    while (pCurrent != nullptr)
    {
        if (pNewNode->getDistanceToCamera(m_pCamera) <= pCurrent->getDistanceToCamera(m_pCamera))
        {
            pNewNode->insertAfter(pPrevious);
            pPrevious = nullptr;
            break;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->next<BBDrawCall>();
    }
    if (pPrevious != nullptr)
    {
        pPrevious->pushBack(pNewNode);
    }
}
