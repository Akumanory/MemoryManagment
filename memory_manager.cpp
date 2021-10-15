#include <sys/mman.h>

#include "memory_manager.h"

MemoryManager::MemoryManager() : 
    fsa16(FSA(16)), 
    fsa32(FSA(32)),
    fsa64(FSA(64)),
    fsa128(FSA(128)),
    fsa256(FSA(256)),
    fsa512(FSA(512))
{}

MemoryManager::~MemoryManager() {
    fsa16.~FSA();
    fsa32.~FSA();
    fsa64.~FSA();
    fsa128.~FSA();
    fsa256.~FSA();
    fsa512.~FSA();
    coalesce.~Coalesce();
}

void MemoryManager::init() {
    fsa16.init();
    fsa32.init();
    fsa64.init();
    fsa128.init();
    fsa256.init();
    fsa512.init();
    coalesce.init();
}

void MemoryManager::destroy() {
    fsa16.destroy();
    fsa32.destroy();
    fsa64.destroy();
    fsa128.destroy();
    fsa256.destroy();
    fsa512.destroy();
    coalesce.destroy();
}

void* MemoryManager::alloc(size_t size) {
    if (size <= 512) {
        if (size <= 64) {
            if (size <= 16) return fsa16.alloc(size);
            if (size <= 32) return fsa32.alloc(size);
            if (size <= 64) return fsa64.alloc(size);
        }
        else {
            if (size <= 128) return fsa128.alloc(size);
            if (size <= 256) return fsa256.alloc(size);
            if (size <= 512) return fsa512.alloc(size);
        }
    }
    else if (size <= PageSize) {
        return coalesce.alloc(size);
    }
    else {
        return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    }
}

void MemoryManager::free(void* p) {
    if (fsa16.contains(p)) return fsa16.free(p);
    if (fsa32.contains(p)) return fsa32.free(p);
    if (fsa64.contains(p)) return fsa64.free(p);
    if (fsa128.contains(p)) return fsa128.free(p);
    if (fsa256.contains(p)) return fsa256.free(p);
    if (fsa512.contains(p)) return fsa512.free(p);
    if (coalesce.contains(p)) return coalesce.free(p);

    // munmap(p, k10mb);
}
