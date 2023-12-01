#ifndef SARE_TEXTURE_H
#define SARE_TEXTURE_H

#include <vector>
#include <string>
#include <d3d9.h>
#include "math_vectors.h"

//-----------------------------------------------------------------------------
class SDL_RWops;

namespace sare
{

//-----------------------------------------------------------------------------
/** @class Texture
*/
class Texture
{
public:
    Texture();
    virtual ~Texture();
    bool Load(SDL_RWops* file);

    bool IsMappedToDevice() const { return device_resource_; }
    bool MapToDevice();
    bool SetAsCurrentInDevice(uint32_t stage);

protected:
    std::vector<uint8_t> data_;
    IDirect3DBaseTexture9* device_resource_;
    bool hardware_;
};

} // end of sare

#endif // SARE_TEXTURE_H

