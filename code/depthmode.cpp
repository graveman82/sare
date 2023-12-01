#include <d3d9.h>

namespace sare {

//-----------------------------------------------------------------------------
bool HasStencil(D3DFORMAT format) {
    switch(format) {
        case D3DFMT_D24S8: return true;
        case D3DFMT_D24X4S4: return true;
        case D3DFMT_D24X8: return false;
        case D3DFMT_D15S1: return true;
        case D3DFMT_D16: return false;
        default: return false;
    }
    return false;
}

} // end of sare

