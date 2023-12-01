#ifndef SARE_STATIC_MESH_H
#define SARE_STATIC_MESH_H

#include <memory>

#include "mesh.h"

namespace sare {

//-----------------------------------------------------------------------------

class StaticMesh : public Mesh {
public:
    StaticMesh();
    ~StaticMesh();
    bool LoadUvws(SDL_RWops* file);
    bool MapToDevice();

private:

    std::vector<D3dxVector3Ex> uvws_;
    IDirect3DVertexBuffer9* uvw_buffer_;
    bool hardware_uvw_;

    TextureSlot2D light_map_;
};

} // end of sare

#endif // SARE_STATIC_MESH_H

