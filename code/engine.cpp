#include "engine.h"
#include <sdl.h>
#include "log.h"
#include <conio.h>
#include <stdio.h>
#include "shared_functions.h"

namespace sare {

static const uint32_t k_sdl_flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS;
//-----------------------------------------------------------------------------
Engine::Engine() : Singleton<Engine>(*this),
    sdl_window_(nullptr),
    sdl_screen_surface_(nullptr),
    direct3d9_(Direct3DCreate9(D3D_SDK_VERSION)),
    direct3ddevice9_(nullptr),
    event_handling_start_time_(0),
    frame_start_time_(0),
    in_render_frame_(false),
    update_period_(1000.0f / 60) {
    if (!direct3d9_) {
        ErrorMessageToUser("No DirectX9 driver installed on your system!");
    }

    HRESULT hr = direct3d9()->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3d_caps9_);
    if (D3D_OK != hr) {
        SafeReleaseCom(direct3d9_);
        ErrorMessageToUser("DirectX9 operation failed when getting capabilities!");
        return;
    }

    if (!(D3DDEVCAPS_HWTRANSFORMANDLIGHT & d3d_caps9_.DevCaps)) {
        SafeReleaseCom(direct3d9_);
        ErrorMessageToUser("No hardware TnL on your video adapter!");
        return;
    }
}

//-----------------------------------------------------------------------------
Engine::~Engine() {
    SafeReleaseCom(direct3d9_);
    Logger& logger = Logger::instance();
    LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Info);
    logger << "Engine::~Engine()" << "\n";
}

//-----------------------------------------------------------------------------
Engine::operator bool() const {
    bool valid = true;
    if (!direct3d9_) valid = false;
    return valid;
}

//-----------------------------------------------------------------------------
void Engine::Run(int argc, char* args[]) {
    if (!Initialize())
        return;
    if (!MainLoop()) {
        return;
    }
    Terminate();
}

//-----------------------------------------------------------------------------
bool Engine::Initialize() {
    if (!InitializeSdl()) {
        return false;
    }

    if (!InitializeRender()) {
        return false;
    }

    if (!material_system().Initialize()) {
        return false;
    }

    if (!InitializeWorld()) {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Engine::InitializeSdl() {
    int sdl_error_code = SDL_Init(k_sdl_flags);
    if (sdl_error_code < 0) {
        HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }
    Logger& logger = Logger::instance();
    LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Info);
    logger << "Sdl was inited.\n";

    sdl_window_ = SDL_CreateWindow( "SARE engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               1024, 768, SDL_WINDOW_SHOWN);
    if (!sdl_window_) {
        HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }
    sdl_screen_surface_ = SDL_GetWindowSurface(sdl_window_);
    logger << "Sdl window was created.\n";
    return true;
}

//-----------------------------------------------------------------------------
void Engine::Terminate() {
    TerminateWorld();
    SafeReleaseCom(direct3ddevice9_);

    if (sdl_window_) {
        SDL_DestroyWindow(sdl_window_);
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Info);
        logger << "Sdl window was destroyed.\n";
    }

    if (SDL_WasInit(k_sdl_flags)) {
        SDL_Quit();
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Info);
        logger << "Sdl was quit.\n";
    }
}
} // end of sare
