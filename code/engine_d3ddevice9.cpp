#include "engine.h"
#include <SDL.h>
#include <SDL_syswm.h>

namespace sare {

//-----------------------------------------------------------------------------
bool Engine::InitializeRender() {
    SARE_ASSERT(!direct3ddevice9_);
    HRESULT hr = direct3d9()->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3d_display_mode_);
    SARE_ASSERT_MSG(D3D_OK == hr, "verify code properly!");

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (!SDL_GetWindowWMInfo(sdl_window_, &info)) {
        HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }

    present_parameters_.Windowed = TRUE;
    present_parameters_.hDeviceWindow = info.info.win.window;
    present_parameters_.BackBufferWidth = 1024;
    present_parameters_.BackBufferHeight = 768;
    present_parameters_.BackBufferCount = 1;
    present_parameters_.BackBufferFormat = D3DFMT_X8R8G8B8;
    present_parameters_.EnableAutoDepthStencil = TRUE;
    present_parameters_.AutoDepthStencilFormat = D3DFMT_D24S8;
    present_parameters_.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    present_parameters_.FullScreen_RefreshRateInHz = 0;
    present_parameters_.MultiSampleType = D3DMULTISAMPLE_NONE;
    present_parameters_.MultiSampleQuality = 0;
    present_parameters_.SwapEffect = D3DSWAPEFFECT_DISCARD;
    present_parameters_.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    DWORD behavior_flags = D3DCREATE_FPU_PRESERVE;

	SARE_ASSERT(D3DDEVCAPS_HWTRANSFORMANDLIGHT & d3d_caps9_.DevCaps);
    if (CanVertexShaderVersion(2, 0)) {
        behavior_flags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
        if (D3DDEVCAPS_PUREDEVICE & d3d_caps9_.DevCaps)
            behavior_flags |= D3DCREATE_PUREDEVICE;
    }
    else {
        behavior_flags |= D3DCREATE_MIXED_VERTEXPROCESSING;
    }

    hr = direct3d9()->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, present_parameters_.hDeviceWindow,
        behavior_flags, &present_parameters_, &direct3ddevice9_);
    if (FAILED(hr)) {
        HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_DirectX9, hr);
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Engine::CanVertexShaderVersion(uint16_t major, uint16_t minor) const {
    return (d3d_caps9_.VertexShaderVersion >= D3DVS_VERSION(major,minor));
}

//-----------------------------------------------------------------------------
bool Engine::CanPixelShaderVersion(uint16_t major, uint16_t minor) const {
    return (d3d_caps9_.PixelShaderVersion >= D3DPS_VERSION(major,minor));
}

} // end of sare

