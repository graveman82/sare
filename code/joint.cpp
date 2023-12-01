#include "joint.h"
#include "assert.h"
#include "file_util.h"

namespace sare {

bool Joint::Load(SDL_RWops* file) {
    SARE_ASSERT(file);
    if (!StdString_Load(name_, file)) {
        return false;
    }

    if (!Matrix4x4_Load(local_tm_, file)) {
        return false;
    }
    return true;
}

} // end of sare
