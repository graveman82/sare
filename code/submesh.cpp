#include "submesh.h"
#include "assert.h"
#include "engine.h"
#include "file_util.h"

namespace sare {

//-----------------------------------------------------------------------------
SubMesh::SubMesh(uint32_t first_vertex, uint32_t vertex_count,
                 uint32_t first_index, uint32_t primitive_count) :
    first_vertex_(first_vertex),
    vertex_count_(vertex_count),
    first_index_(first_index),
    primitive_count_(primitive_count) {
}

//-----------------------------------------------------------------------------
SubMesh::~SubMesh() {
}

//-----------------------------------------------------------------------------
bool SubMesh::Load(SDL_RWops* file) {
    SARE_ASSERT(file);

    if (!Uint16_Load(first_index_, file)) {
        return false;
    }
    if (!Uint16_Load(primitive_count_, file)) {
        return false;
    }

    // Texture map info
    if (!diffuse_map_.Load(file)) {
        return false;
    }
    TextureSlot2D texture_slot;
    texture_slot.Load(file);
    texture_slot.Load(file);
    texture_slot.Load(file);
    return true;
}

//-----------------------------------------------------------------------------
void SubMesh::SetVertexRange(uint16_t first_vertex, uint16_t vertex_count) {
    first_vertex_ = first_vertex;
    vertex_count_ = vertex_count;
}

//-----------------------------------------------------------------------------
bool SubMesh::Render() {
    if (!diffuse_map_.SetAsCurrentInDevice(0)) {
        return false;
    }
    HRESULT hr = Engine::instance().direct3ddevice9()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
        0, first_vertex_, vertex_count_, first_index_, primitive_count_);
    SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");
    return true;
}

} // end of sare
