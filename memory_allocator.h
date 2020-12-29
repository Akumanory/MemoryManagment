#pragma once

#include <iostream>
#include <sys/mman.h>

constexpr int PageSize = 4096;

class MemoryAllocator
{
public:
    virtual ~MemoryAllocator() = 0;

    virtual void init() = 0;
    virtual void destroy() = 0;

    virtual void* alloc(size_t size) = 0;
    virtual void free(void* p) = 0;
};