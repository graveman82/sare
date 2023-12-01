#ifndef SARE_MEMORYMANAGER_H
#define SARE_MEMORYMANAGER_H

#include <stdint.h>
#include <memory>

class SrMemoryManager
{
public:
    typedef uint8_t SrByte;
    typedef uint32_t SrSize;

    void* Allocate(SrSize nBytes);
    void* Reallocate(void* p, SrSize n);
    void Free(void* p);

};

typedef std::shared_ptr<SrMemoryManager> SrMemoryManagerSPtr;
#endif // SARE_MEMORYMANAGER_H

