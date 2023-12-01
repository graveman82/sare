#ifndef SARE_TEXTURE_SLOT_H_
#define SARE_TEXTURE_SLOT_H_

#include <string>
#include <d3d9.h>
#include "math_vectors.h"

//-----------------------------------------------------------------------------
class SDL_RWops;

namespace sare {

//-----------------------------------------------------------------------------
/** @class TextureSlot
*/
class TextureSlot {
public:
    TextureSlot();
    virtual ~TextureSlot();
    bool Load(SDL_RWops* file);
    virtual bool MapToDevice(const char* path) = 0;
    virtual bool SetAsCurrentInDevice(uint32_t stage) = 0;
    const char* fileName() const { return file_name_.c_str(); }

protected:
    std::string file_name_;
    D3dxVector2Ex translate_;
    D3dxVector2Ex scale_;
    IDirect3DBaseTexture9* texture_;
    bool hardware_;
};

//-----------------------------------------------------------------------------
/** @class TextureSlot2D
*/
class TextureSlot2D : public TextureSlot {
public:
    bool MapToDevice(const char* path);
    bool SetAsCurrentInDevice(uint32_t stage);
};

} // end of sare
#endif // SARE_TEXTURE_SLOT_H_

