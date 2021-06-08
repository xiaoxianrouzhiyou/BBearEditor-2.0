#include "BBProfiler.h"


QMap<void*, BBMemoryLabel> BBProfiler::m_MemoryObjects;
QMap<void*, size_t> BBProfiler::m_MemoryObjectsSize;
QMap<BBMemoryLabel, int> BBProfiler::m_UsedMemorySize;
int BBProfiler::m_nTotalUsedMemorySize = 0;


void BBProfiler::addMemoryObject(void *ptr, BBMemoryLabel eLabel, size_t size)
{
    m_MemoryObjects.insert(ptr, eLabel);
    m_MemoryObjectsSize.insert(ptr, size);
    m_nTotalUsedMemorySize += size;
    if (m_UsedMemorySize.find(eLabel) != m_UsedMemorySize.end())
    {
        m_UsedMemorySize[eLabel] += size;
    }
    else
    {
        m_UsedMemorySize.insert(eLabel, size);
    }
}

bool BBProfiler::deleteMemoryObject(void *ptr)
{
    QMap<void*, BBMemoryLabel>::Iterator iter = m_MemoryObjects.find(ptr);
    if (iter != m_MemoryObjects.end())
    {
        auto sizeIter = m_MemoryObjectsSize.find(ptr);
        int size = sizeIter.value();
        m_UsedMemorySize[iter.value()] -= size;
        m_nTotalUsedMemorySize -= size;
        m_MemoryObjectsSize.erase(sizeIter);
        m_MemoryObjects.erase(iter);
        return true;
    }
    return false;
}

int BBProfiler::getTotalUsedMemorySize()
{
    return m_nTotalUsedMemorySize;
}
