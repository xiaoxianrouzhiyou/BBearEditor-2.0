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

    m_bVisible = true;
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

void BBDrawCall::updateOrderInRenderQueue(const QVector3D &renderableObjectPosition)
{
    m_RenderableObjectPosition = renderableObjectPosition;
    // for the time, only consider opaque
    BBRenderQueue *pRenderQueue = BBSceneManager::getRenderQueue();
    pRenderQueue->updateOpaqueDrawCallOrder(this);
}

float BBDrawCall::getDistanceToCamera(BBCamera *pCamera)
{
    return m_RenderableObjectPosition.distanceToPoint(pCamera->getPosition());
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
    if (m_bVisible)
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
    }

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

void BBDrawCall::uiRendering(BBCanvas *pCanvas)
{
    m_pVBO->bind();
    m_pMaterial->getBaseRenderPass()->bind(pCanvas);
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
        m_pOpaqueDrawCall->pushBack(pDC);
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
        m_pTransparentDrawCall->pushBack(pDC);
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
        if (m_pOpaqueDrawCall == pDC)
        {
            m_pOpaqueDrawCall = m_pOpaqueDrawCall->removeSelf<BBDrawCall>();
        }
        else
        {
            m_pOpaqueDrawCall->remove(pDC);
        }
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

void BBRenderQueue::updateOrder()
{
    updateOpaqueDrawCallOrder();
    // to do ...
}

void BBRenderQueue::updateOpaqueDrawCallOrder()
{
    BB_PROCESS_ERROR_RETURN(m_pOpaqueDrawCall);
    // reorder the nodes in the pHead into another queue
    // There is only one node in this queue, so there is no need to update the order
    if (!m_pOpaqueDrawCall->isEnd())
    {
        BBDrawCall *pCurrent = m_pOpaqueDrawCall;
        // remove pCurrent from old queue
        BBDrawCall *pNext = pCurrent->removeSelf<BBDrawCall>();
        BBDrawCall *pNewHead = pCurrent;
        // start from the second in the old queue
        pCurrent = pNext;
        while (pCurrent)
        {
            pNext = pCurrent->removeSelf<BBDrawCall>();
            // insert in new queue orderly
            pNewHead = appendAscendingRenderQueue(pNewHead, pCurrent);
            pCurrent = pNext;
        }
        m_pOpaqueDrawCall = pNewHead;
    }
}

void BBRenderQueue::updateOpaqueDrawCallOrder(BBDrawCall *pNode)
{
    BB_PROCESS_ERROR_RETURN(m_pOpaqueDrawCall);
    // There is only one node in this queue, so there is no need to update the order
    if (!m_pOpaqueDrawCall->isEnd())
    {
        if (m_pOpaqueDrawCall == pNode)
        {
            // remove head and make next as head
            m_pOpaqueDrawCall = pNode->removeSelf<BBDrawCall>();
        }
        else
        {
            // remove the node from the queue
            m_pOpaqueDrawCall->remove(pNode);
        }
        // reinsert in the new pos
        m_pOpaqueDrawCall = appendAscendingRenderQueue(m_pOpaqueDrawCall, pNode);
    }
}

/**
 * @brief BBRenderQueue::appendAscendingRenderQueue
 * @param pHead
 * @param pNewNode
 * @return                                              new head
 */
BBDrawCall* BBRenderQueue::appendAscendingRenderQueue(BBDrawCall *pHead, BBDrawCall *pNewNode)
{
    BBDrawCall *pCurrent = pHead;
    BBDrawCall *pPrevious = nullptr;
    while (pCurrent != nullptr)
    {
        if (pNewNode->getDistanceToCamera(m_pCamera) <= pCurrent->getDistanceToCamera(m_pCamera))
        {
            if (pPrevious == nullptr)
            {
                // change head
                pNewNode->pushBack(pCurrent);
                return pNewNode;
            }
            else
            {
                pNewNode->insertAfter(pPrevious);
                pPrevious = nullptr;
                break;
            }
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->next<BBDrawCall>();
    }
    if (pPrevious != nullptr)
    {
        pPrevious->pushBack(pNewNode);
    }
    return pHead;
}
