#include "file_util.h"
#include "assert.h"
#include "log.h"
#include "engine.h"

namespace sare {

//-----------------------------------------------------------------------------
bool Uint16_Load(uint16_t& u, SDL_RWops* file) {
    SARE_ASSERT(file);
    if (!SDL_RWread(file, &u, 2, 1)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool Uint32_Load(uint32_t& u, SDL_RWops* file) {
    SARE_ASSERT(file);
    if (!SDL_RWread(file, &u, 4, 1)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

bool StdString_Load(std::string& str, SDL_RWops* file) {
    SARE_ASSERT(file);
    uint32_t len;
    if (!SDL_RWread(file, &len, 4, 1)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }

    static std::vector<char> buf;
    buf.clear();
    buf.resize(len);
    if (!SDL_RWread(file, &buf[0], 1, len)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }

    buf.push_back(0);
    str = &buf[0];
    return true;
}

//-----------------------------------------------------------------------------
bool Vector2D_Load(D3DXVECTOR2& v, SDL_RWops* file) {
    if (!SDL_RWread(file, &v.x, 4, 2)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool Vector3D_Load(D3DXVECTOR3& v, SDL_RWops* file) {
    if (!SDL_RWread(file, &v.x, 4, 3)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool Vector4D_Load(D3DXVECTOR4& v, SDL_RWops* file) {
    if (!SDL_RWread(file, &v.x, 4, 4)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool Matrix4x4_Load(D3DXMATRIX& matrix, SDL_RWops* file) {
    SARE_ASSERT(file);
    if (!SDL_RWread(file, &matrix.m[0][0], 4, 16)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool D3dxVector3Ex_Load(D3dxVector3Ex* v, uint32_t count, SDL_RWops* file) {
    SARE_ASSERT(file);
    if (!SDL_RWread(file, v, sizeof(D3dxVector3Ex), count)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool VertexArray_Load(Vertex* v, uint32_t count, SDL_RWops* file) {
    SARE_ASSERT(file);
    if (!SDL_RWread(file, v, sizeof(Vertex), count)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Uint16Array_Load(uint16_t* v, uint32_t count, SDL_RWops* file) {
    SARE_ASSERT(file);
    if (!SDL_RWread(file, v, sizeof(uint16_t), count)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }
    return true;
}
} // end of sare
