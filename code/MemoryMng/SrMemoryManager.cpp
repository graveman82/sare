#include "MemoryMng/SrMemoryManager.h"
#include <cstdlib>

void* SrMemoryManager::Allocate(SrSize nBytes)
{
    return std::malloc(nBytes);
}

void* SrMemoryManager::Reallocate(void* p, SrSize n)
{
    return std::realloc(p, n);
}

void SrMemoryManager::Free(void* p)
{
    std::free(p);
}
