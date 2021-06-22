#include "BBProfiler.h"
#include "Utils/BBUtils.h"


QMap<void*, BBMemoryLabel>* BBProfiler::m_pMemoryObjects = NULL;
QMap<void*, size_t>* BBProfiler::m_pMemoryObjectsSize = NULL;
QMap<BBMemoryLabel, int>* BBProfiler::m_pUsedMemorySize = NULL;
int BBProfiler::m_nTotalUsedMemorySize = 0;


void BBProfiler::init()
{
    static QMap<void*, BBMemoryLabel> memoryObjects = BBStatic::var<0, QMap<void*, BBMemoryLabel>>();
    static QMap<void*, size_t> memoryObjectsSize = BBStatic::var<0, QMap<void*, size_t>>();
    static QMap<BBMemoryLabel, int> usedMemorySize = BBStatic::var<0, QMap<BBMemoryLabel, int>>();
    m_pMemoryObjects = &memoryObjects;
    m_pMemoryObjectsSize = &memoryObjectsSize;
    m_pUsedMemorySize = &usedMemorySize;
}

void BBProfiler::addMemoryObject(void *ptr, const BBMemoryLabel &eLabel, size_t size)
{
    if (!m_pMemoryObjects || !m_pMemoryObjectsSize || !m_pUsedMemorySize)
    {
        init();
    }
    m_pMemoryObjects->insert(ptr, eLabel);
    m_pMemoryObjectsSize->insert(ptr, size);
    m_nTotalUsedMemorySize += size;
    if (m_pUsedMemorySize->find(eLabel) != m_pUsedMemorySize->end())
    {
        (*m_pUsedMemorySize)[eLabel] += size;
    }
    else
    {
        m_pUsedMemorySize->insert(eLabel, size);
    }
}

bool BBProfiler::deleteMemoryObject(void *ptr)
{
    QMap<void*, BBMemoryLabel>::Iterator iter = m_pMemoryObjects->find(ptr);
    if (iter != m_pMemoryObjects->end())
    {
        auto sizeIter = m_pMemoryObjectsSize->find(ptr);
        int size = sizeIter.value();
        (*m_pUsedMemorySize)[iter.value()] -= size;
        m_nTotalUsedMemorySize -= size;
        m_pMemoryObjectsSize->erase(sizeIter);
        m_pMemoryObjects->erase(iter);
        return true;
    }
    return false;
}

int BBProfiler::getTotalUsedMemorySize()
{
    return m_nTotalUsedMemorySize;
}
