#include "BBLinkedList.h"

BBLinkedList::BBLinkedList()
{
    m_pNext = nullptr;
}

void BBLinkedList::pushBack(BBLinkedList *pNode)
{
    if (m_pNext == nullptr)
    {
        m_pNext = pNode;
    }
    else
    {
        m_pNext->pushBack(pNode);
    }
}

void BBLinkedList::remove(BBLinkedList *pNode)
{
    if (m_pNext == pNode)
    {
        m_pNext = m_pNext->m_pNext;
        pNode->m_pNext = nullptr;
    }
    else
    {
        m_pNext->remove(pNode);
    }
}
