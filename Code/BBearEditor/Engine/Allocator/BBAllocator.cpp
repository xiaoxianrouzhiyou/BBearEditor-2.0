#include "BBAllocator.h"
#include "tlsf/tlsf.h"
#include "stdlib.h"
#include "Profiler/BBProfiler.h"


// 1024             1KB
// 1024x1024        1MB     1048576
// 1024x1024x1024   1GB     1073741824

static tlsf_t g_TLSF = NULL;
static unsigned int g_ReservedSize = 209715200; // 200MB


static void initMemory()
{
    if (g_TLSF == NULL)
    {
        void *pBuffer = malloc(g_ReservedSize);
        g_TLSF = tlsf_create_with_pool(pBuffer, g_ReservedSize);
    }
}

static void* getMemory(std::size_t size)
{
    if (g_TLSF == NULL)
    {
        initMemory();
    }
    void *pMemory = tlsf_malloc(g_TLSF, size);
    while (pMemory == NULL)
    {
        void *pAddPool = malloc(104857600); // 100MB
        tlsf_add_pool(g_TLSF, pAddPool, 104857600);
        pMemory = tlsf_malloc(g_TLSF, size);
    }
    return pMemory;
}

static void recycle(void *ptr)
{
#if BB_USE_POOL
    if (BBProfiler::deleteMemoryObject(ptr))
    {
        tlsf_free(g_TLSF, ptr);
    }
    else
    {
        free(ptr);
    }
#else
    free(ptr);
#endif
}

void *operator new(std::size_t size)
{
#if BB_USE_POOL
    void *ptr = getMemory(size);
    BBProfiler::addMemoryObject(ptr, BBMemoryLabel::Default, tlsf_block_size(ptr));
    return ptr;
#else
    void *ptr = malloc(size);
    return ptr;
#endif
}

void *operator new[](std::size_t size)
{
#if BB_USE_POOL
    void *ptr = getMemory(size);
    BBProfiler::addMemoryObject(ptr, BBMemoryLabel::Default, tlsf_block_size(ptr));
    return ptr;
#else
    void *ptr = malloc(size);
    return ptr;
#endif
}

void *operator new(std::size_t size, BBMemoryLabel eID)
{
#if BB_USE_POOL
    void *ptr = getMemory(size);
    BBProfiler::addMemoryObject(ptr, eID, tlsf_block_size(ptr));
    return ptr;
#else
    void *ptr = malloc(size);
    return ptr;
#endif
}

void *operator new[](std::size_t size, BBMemoryLabel eID)
{
#if BB_USE_POOL
    void *ptr = getMemory(size);
    BBProfiler::addMemoryObject(ptr, eID, tlsf_block_size(ptr));
    return ptr;
#else
    void *ptr = malloc(size);
    return ptr;
#endif
}

void operator delete(void *ptr) noexcept
{
    recycle(ptr);
}

void operator delete[](void *ptr) noexcept
{
    recycle(ptr);
}

void operator delete(void *ptr, BBMemoryLabel eID) noexcept
{
    recycle(ptr);
}

void operator delete[](void *ptr, BBMemoryLabel eID) noexcept
{
    recycle(ptr);
}
