#ifndef SARE_MESH_H
#define SARE_MESH_H

#include <vector>
#include <string>
#include <stdint.h>

#include <d3d9.h>

#include "math_vectors.h"
#include "vertex.h"
#include "submesh.h"

namespace sare
{

//-----------------------------------------------------------------------------
/** @class
*/
class Mesh
{
public:
    using VertexVector = std::vector<Vertex>;
    using Index = uint16_t;
    using IndexVector = std::vector<Index>;
    Mesh();
    virtual ~Mesh();

    bool LoadVertices(SDL_RWops* file);
    bool LoadIndices(SDL_RWops* file);
    bool LoadSubMeshes(SDL_RWops* file);
    bool SaveToX(const char* file_name);

    void Dump();

    virtual bool MapToDevice();
    bool MapToDeviceTextures(const char* directory);
    bool SetAsCurrentInDevice();
    bool Render();

    bool CreateCube(const D3dxVector3Ex& size);

    const VertexVector& vertices() const { return vertices_; }
    const IndexVector& indices() const { return indices_; }
    uint16_t TriCount() const { return indices().size() / 3; }

protected:

    void UpdateVertexRanges();

    VertexVector vertices_;
    IDirect3DVertexBuffer9* vertex_buffer_;
    bool hardware_vb_;

    IndexVector indices_;
    IDirect3DIndexBuffer9* index_buffer_;
    bool hardware_ib_;

    std::vector<SubMesh> sub_meshes_;
};

} // end of sare
#endif // SARE_MESH_H
