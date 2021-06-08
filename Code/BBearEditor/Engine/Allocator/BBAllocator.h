#pragma once

#ifndef BBALLOCATOR_H
#define BBALLOCATOR_H


#define BB_USE_POOL 1
#include <new>
#include "bbmemorylabel.h"


static void initMemory();
static void* getMemory(std::size_t size);
static void recycle(void *ptr);


void *operator new(std::size_t size);
void *operator new[](std::size_t size);
void *operator new(std::size_t size, BBMemoryLabel eID);
void *operator new[](std::size_t size, BBMemoryLabel eID);


void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;
void operator delete(void *ptr, BBMemoryLabel eID) noexcept;
void operator delete[](void *ptr, BBMemoryLabel eID) noexcept;


#endif // BBALLOCATOR_H
