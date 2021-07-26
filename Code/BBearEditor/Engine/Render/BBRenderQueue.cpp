#include "BBRenderQueue.h"
#include "Utils/BBUtils.h"
#include "BBDrawCall.h"


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
        if (m_pTransparentDrawCall == pDC)
        {
            m_pTransparentDrawCall = m_pTransparentDrawCall->removeSelf<BBDrawCall>();
        }
        else
        {
            m_pTransparentDrawCall->remove(pDC);
        }
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

void BBRenderQueue::updateAllDrawCallOrder()
{
    updateAllOpaqueDrawCallOrder();
    updateAllTransparentDrawCallOrder();
    // to do ...
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

void BBRenderQueue::updateTransparentDrawCallOrder(BBDrawCall *pNode)
{
    BB_PROCESS_ERROR_RETURN(m_pTransparentDrawCall);
    // There is only one node in this queue, so there is no need to update the order
    if (!m_pTransparentDrawCall->isEnd())
    {
        if (m_pTransparentDrawCall == pNode)
        {
            // remove head and make next as head
            m_pTransparentDrawCall = pNode->removeSelf<BBDrawCall>();
        }
        else
        {
            // remove the node from the queue
            m_pTransparentDrawCall->remove(pNode);
        }
        // reinsert in the new pos
        m_pTransparentDrawCall = appendDescendingRenderQueue(m_pTransparentDrawCall, pNode);
    }
}

void BBRenderQueue::switchQueue(BBMaterial *pPrevious, BBMaterial *pCurrent, BBDrawCall *pDrawCall)
{
    bool bPreviousBlendState = pPrevious->getBlendState();
    bool bCurrentBlendState = pCurrent->getBlendState();
    BB_PROCESS_ERROR_RETURN(bPreviousBlendState != bCurrentBlendState);
    switchQueue(!bPreviousBlendState, pDrawCall);
}

void BBRenderQueue::switchQueue(bool bEnableBlend, BBDrawCall *pDrawCall)
{
    if (bEnableBlend)
    {
        // false -> true
        // opaque -> transparent
        removeOpaqueDrawCall(pDrawCall);
        appendTransparentDrawCall(pDrawCall);
    }
    else
    {
        // true -> false
        // transparent -> opaque
        removeTransparentDrawCall(pDrawCall);
        appendOpaqueDrawCall(pDrawCall);
    }
}

void BBRenderQueue::updateAllOpaqueDrawCallOrder()
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

void BBRenderQueue::updateAllTransparentDrawCallOrder()
{
    BB_PROCESS_ERROR_RETURN(m_pTransparentDrawCall);
    // reorder the nodes in the pHead into another queue
    // There is only one node in this queue, so there is no need to update the order
    if (!m_pTransparentDrawCall->isEnd())
    {
        BBDrawCall *pCurrent = m_pTransparentDrawCall;
        // remove pCurrent from old queue
        BBDrawCall *pNext = pCurrent->removeSelf<BBDrawCall>();
        BBDrawCall *pNewHead = pCurrent;
        // start from the second in the old queue
        pCurrent = pNext;
        while (pCurrent)
        {
            pNext = pCurrent->removeSelf<BBDrawCall>();
            // insert in new queue orderly
            pNewHead = appendDescendingRenderQueue(pNewHead, pCurrent);
            pCurrent = pNext;
        }
        m_pTransparentDrawCall = pNewHead;
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

BBDrawCall* BBRenderQueue::appendDescendingRenderQueue(BBDrawCall *pHead, BBDrawCall *pNewNode)
{
    BBDrawCall *pCurrent = pHead;
    BBDrawCall *pPrevious = nullptr;
    while (pCurrent != nullptr)
    {
        if (pNewNode->getDistanceToCamera(m_pCamera) >= pCurrent->getDistanceToCamera(m_pCamera))
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

