#include <sdl.h>

#include "texture_slot.h"
#include "assert.h"
#include "engine.h"
#include "shared_functions.h"
#include "file_util.h"

namespace sare {

//-----------------------------------------------------------------------------
TextureSlot::TextureSlot() :
    scale_(1.0f, 1.0f),
    texture_(nullptr),
    hardware_(false) {
}

//-----------------------------------------------------------------------------
TextureSlot::~TextureSlot() {
    SafeReleaseCom(texture_);
}

//-----------------------------------------------------------------------------
bool TextureSlot::Load(SDL_RWops* file) {
    SARE_ASSERT(file);
    if (!StdString_Load(file_name_, file)) {
        return false;
    }

    if (!Vector2D_Load(translate_, file)) {
        return false;
    }
    //printf("        translate: %8.3f, %8.3f\n", translate_.x, translate_.y);

    if (!Vector2D_Load(scale_, file)) {
        return false;
    }
    //printf("        scale: %8.3f, %8.3f\n", scale_.x, scale_.y);

    return true;
}

//-----------------------------------------------------------------------------
bool TextureSlot2D::MapToDevice(const char* path) {
    if (file_name_.empty())
        return true;
    SARE_ASSERT(path);
    static std::string file_name;
    file_name = path;
    file_name += file_name_;
    file_name += ".dds";
//printf("TextureSlot2D::MapToDevice. filename: %s\n", file_name.c_str());

    IDirect3DTexture9* direct3dtexture9 = nullptr;
    HRESULT hr = D3DXCreateTextureFromFile(Engine::instance().direct3ddevice9(),
                    file_name.c_str(), &direct3dtexture9);
    if (D3D_OK == hr) {
        texture_ = direct3dtexture9;
        hardware_ = true;
        return true;
    }

    if (D3DERR_OUTOFVIDEOMEMORY == hr) {
        direct3dtexture9 = nullptr;
        hr = D3DXCreateTextureFromFileEx(Engine::instance().direct3ddevice9(),
                file_name.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
                D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_BOX | D3DX_FILTER_DITHER,
                D3DX_DEFAULT, 0, nullptr, nullptr, &direct3dtexture9);
    }

    if (D3D_OK == hr) {
        texture_ = direct3dtexture9;
        return true;
    }
    else if (E_OUTOFMEMORY == hr) {
        Engine::instance().HandleError(kErrorCode_NotEnoughMemory, kErrorLevel_Fatal);
    }
    else {
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_DirectX9, hr);
    }
    return false;
}

//-----------------------------------------------------------------------------
bool TextureSlot2D::SetAsCurrentInDevice(uint32_t stage) {
    HRESULT hr = Engine::instance().direct3ddevice9()->SetTexture(stage, texture_);
    SARE_ASSERT(D3D_OK == hr);
    return true;
}
} // end of sare
