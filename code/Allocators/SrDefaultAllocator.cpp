#include "Allocators/SrDefaultAllocator.h"
#include "engine.h"


SrMemoryManagerSPtr SrDefaultAllocator_GetMemMng()
{
    return sare::Engine::instance().GetMemoryManager();
}
