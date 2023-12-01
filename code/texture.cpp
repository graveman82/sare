#include <sdl.h>

#include "texture.h"
#include "assert.h"
#include "engine.h"
#include "shared_functions.h"
#include "file_util.h"
#include "log.h"

namespace sare
{

//-----------------------------------------------------------------------------
Texture::Texture() :
    device_resource_(nullptr),
    hardware_(false)
{
}

//-----------------------------------------------------------------------------
Texture::~Texture()
{
    SafeReleaseCom(device_resource_);
}

//-----------------------------------------------------------------------------
bool Texture::Load(SDL_RWops* file)
{
    SARE_ASSERT(file);
    uint32_t file_size = (uint32_t) SDL_RWsize(file);
    if (uint32_t(-1) == file_size)
    {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWsize() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }

    data_.resize(file_size);
    if (!SDL_RWread(file, &data_[0], 1, file_size))
    {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWread() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        data_.clear();
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool Texture::MapToDevice()
{
    if (device_resource_)
        return false;

    if (data_.empty())
        return false;

    IDirect3DTexture9* direct3dtexture9 = nullptr;
    HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(Engine::instance().direct3ddevice9(),
                    &data_[0], data_.size(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
                D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_BOX | D3DX_FILTER_DITHER,
                D3DX_DEFAULT, 0, nullptr, nullptr, &direct3dtexture9);

    if (D3D_OK == hr)
    {
        device_resource_ = direct3dtexture9;
        hardware_ = true;
        return true;
    }

    if (D3DERR_OUTOFVIDEOMEMORY == hr)
    {
        direct3dtexture9 = nullptr;
        hr = D3DXCreateTextureFromFileInMemoryEx(Engine::instance().direct3ddevice9(),
                    &data_[0], data_.size(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
                D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_BOX | D3DX_FILTER_DITHER,
                D3DX_DEFAULT, 0, nullptr, nullptr, &direct3dtexture9);
    }

    if (D3D_OK == hr)
    {
        device_resource_ = direct3dtexture9;
        hardware_ = false;
        return true;
    }
    else if (E_OUTOFMEMORY == hr)
    {
        Engine::instance().HandleError(kErrorCode_NotEnoughMemory, kErrorLevel_Fatal);
    }
    else
    {
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_DirectX9, hr);
    }
    return false;
}

//-----------------------------------------------------------------------------
bool Texture::SetAsCurrentInDevice(uint32_t stage)
{
    HRESULT hr = Engine::instance().direct3ddevice9()->SetTexture(stage, device_resource_);
    SARE_ASSERT(D3D_OK == hr);
    return true;
}
} // end of sare
