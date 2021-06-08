#pragma once

#ifndef BBPROFILER_H
#define BBPROFILER_H


#include "Allocator/bbmemorylabel.h"
#include <QMap>
#include <cstddef>


class BBProfiler
{
public:
    static void addMemoryObject(void *ptr, BBMemoryLabel eLabel, size_t size);
    static bool deleteMemoryObject(void *ptr);
    static int getTotalUsedMemorySize();

    static QMap<void*, BBMemoryLabel> m_MemoryObjects;
    static QMap<void*, size_t> m_MemoryObjectsSize;
    static QMap<BBMemoryLabel, int> m_UsedMemorySize;
    static int m_nTotalUsedMemorySize;
};

#endif // BBPROFILER_H
