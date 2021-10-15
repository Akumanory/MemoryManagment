#pragma once

// #include "memory_allocator.h"
#include "fsa_old.cpp"
#include "coalesce_old.cpp"

class MemoryManager final : public MemoryAllocator
{
private:
    FSA fsa16;
    FSA fsa32;
    FSA fsa64;
    FSA fsa128;
    FSA fsa256;
    FSA fsa512;

    Coalesce coalesce;

public:
    MemoryManager();
    ~MemoryManager();

    void init() override;
    void destroy() override;

    void* alloc(size_t size) override;
    void free(void* p) override;

};