#pragma once

#ifndef BBPROFILER_H
#define BBPROFILER_H


#include "Allocator/bbmemorylabel.h"
#include <QMap>
#include <cstddef>


class BBProfiler
{
public:
    static void init();
    static void addMemoryObject(void *ptr, const BBMemoryLabel &eLabel, size_t size);
    static bool deleteMemoryObject(void *ptr);
    static int getTotalUsedMemorySize();

    static QMap<void*, BBMemoryLabel> *m_pMemoryObjects;
    static QMap<void*, size_t> *m_pMemoryObjectsSize;
    static QMap<BBMemoryLabel, int> *m_pUsedMemorySize;
    static int m_nTotalUsedMemorySize;
};

#endif // BBPROFILER_H
