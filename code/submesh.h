#ifndef SARE_SUBMESH_H
#define SARE_SUBMESH_H

#include <stdint.h>
#include "texture_slot.h"

namespace sare {

//-----------------------------------------------------------------------------
/** @class SubMesh Подмножество сетки.
*/
class SubMesh {
public:
    SubMesh(uint32_t first_vertex = 0, uint32_t vertex_count = 0,
            uint32_t first_index = 0, uint32_t primitive_count = 0);
    ~SubMesh();
    bool Load(SDL_RWops* file);
    void SetVertexRange(uint16_t first_vertex, uint16_t vertex_count);
    bool Render();

    uint16_t first_vertex() const { return first_vertex_; }
    uint16_t vertex_count() const { return vertex_count_; }
    uint16_t first_index() const { return first_index_; }
    uint16_t primitive_count() const { return primitive_count_; }
    TextureSlot2D& diffuse_map() { return diffuse_map_; }
    const TextureSlot2D& diffuse_map() const { return diffuse_map_; }

private:
    uint16_t first_vertex_;
    uint16_t vertex_count_;
    uint16_t first_index_;
    uint16_t primitive_count_;

    TextureSlot2D diffuse_map_;
};

} // end of sare
#endif // SARE_SUBMESH_H

