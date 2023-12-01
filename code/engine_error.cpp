#include "engine.h"
#include <sdl.h>
#include "log.h"

namespace sare {

bool Engine::HandleError(ErrorCode error_code, ErrorLevel error_level,
                         ExternalApi external_api, long api_error_code) {
    if (kErrorCode_ApiInternal == error_code) {
        if (kExternalApi_Sdl2 == external_api) {
            if (kErrorLevel_Fatal == error_level) {
                Logger& logger = Logger::instance();
                LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Fatal);
                logger << SDL_GetError() << "\n";
                Terminate();
                return true;
            }
        }
        else if (kExternalApi_DirectX9 == external_api) {
            if (kErrorLevel_Fatal == error_level) {
                Logger& logger = Logger::instance();
                LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Fatal);
                logger << "DirectX9 operation failed with error code: " << uint32_t(api_error_code) << "!\n";
                Terminate();
                return true;
            }

        }

    }
    else if (kErrorCode_NotEnoughMemory == error_code) {
        ::MessageBoxA(0, "Not enough memory on your system!",
                      "Error", MB_OK | MB_TOPMOST |MB_ICONERROR);
        Terminate();
        return true;
    }
    else if (kErrorCode_MalformedData == error_code) {
        ::MessageBoxA(0, "Game content files was malformed!",
                      "Error", MB_OK | MB_TOPMOST |MB_ICONERROR);
        Terminate();
        return true;
    }

    return false;
}

} // end of sare
