#include "engine.h"

namespace sare {

//-----------------------------------------------------------------------------
LPDIRECT3D9 Engine::direct3d9() {
    SARE_ASSERT(direct3d9_);
    return direct3d9_;
}

//-----------------------------------------------------------------------------
LPDIRECT3DDEVICE9 Engine::direct3ddevice9() {
    SARE_ASSERT(direct3ddevice9_);
    return direct3ddevice9_;
}

//-----------------------------------------------------------------------------
MaterialSystem& Engine::material_system() {
    return material_system_;
}

//-----------------------------------------------------------------------------
Camera& Engine::camera() {
    return camera_;
}

} // end of sare
