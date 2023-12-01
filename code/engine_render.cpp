#include "engine.h"
#include "log.h"

namespace sare {

bool HasStencil(D3DFORMAT format);

//-----------------------------------------------------------------------------
bool Engine::Draw() {

    if (!BeginRenderFrame()) {
        return false;
    }

    if (!ClearBackBuffer(D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
                                                        RgbaColori(0, 0, 100, 0xff), 1.0f, 0)) {
        return false;
    }

    DrawWorld();

    if (!EndRenderFrame()) {
        return false;
    }

    if (!RenderPresent()) {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Engine::BeginRenderFrame() {

    HRESULT hr = direct3ddevice9()->BeginScene();
    if (FAILED(hr)) {
        if (D3DERR_INVALIDCALL == hr) {
            SARE_ASSERT_MSG(!in_render_frame_, "nested beginscene-endscene pairs is not allowed!");

        }
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "Engine::BeginRenderFrame() failed!\n";
        HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_DirectX9, hr);
        return false;
    }

    in_render_frame_ = true;
    return true;
}

//-----------------------------------------------------------------------------
bool Engine::EndRenderFrame() {

    HRESULT hr = direct3ddevice9()->EndScene();
    if (FAILED(hr)) {
        if (D3DERR_INVALIDCALL == hr) {
            SARE_ASSERT_MSG(in_render_frame_, "call only once after Engine::BeginRenderFrame()!");
        }
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "Engine::EndRenderFrame() failed!\n";
        HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_DirectX9, hr);
        return false;
    }
    in_render_frame_ = false;
    return true;
}

//-----------------------------------------------------------------------------
bool Engine::RenderPresent() {

    HRESULT hr = direct3ddevice9()->Present(nullptr, nullptr, 0, nullptr);
    if (FAILED(hr)) {
        if (D3DERR_INVALIDCALL == hr) {
            SARE_ASSERT_MSG(!in_render_frame_, "call only once after Engine::EndRenderFrame()!");
        }
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "Engine::RenderPresent() failed!\n";
        HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_DirectX9, hr);
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Engine::ClearBackBuffer(uint32_t clear_flags, RgbaColori color, float z, int stencil) {

    if (clear_flags & D3DCLEAR_ZBUFFER) {
        SARE_ASSERT(present_parameters().EnableAutoDepthStencil);
    }
    if (clear_flags & D3DCLEAR_STENCIL) {
        SARE_ASSERT(present_parameters().EnableAutoDepthStencil);
        SARE_ASSERT(HasStencil(present_parameters().AutoDepthStencilFormat));
    }

    D3DCOLOR d3d_color = D3DCOLOR_RGBA(color.r, color.g, color.b, color.a);

    HRESULT hr = direct3ddevice9()->Clear(0, nullptr, clear_flags, d3d_color, z, stencil);
    if (FAILED(hr)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "Engine::::ClearBackBuffer() failed!\n";
        HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_DirectX9, hr);
        return false;
    }
    return true;
}


} // end of sare
