#include <sdl.h>

#include "mesh.h"
#include "engine.h"
#include "log.h"
#include "shared_functions.h"
#include "file_util.h"

#include <d3dx9.h>
#include <memory>

namespace sare {


//-----------------------------------------------------------------------------
//
static IDirect3DVertexDeclaration9* s_vertex_declaration = nullptr;

//-----------------------------------------------------------------------------
Mesh::Mesh() :
    vertex_buffer_(nullptr),
    hardware_vb_(false),
    index_buffer_(nullptr),
    hardware_ib_(false) {
}

//-----------------------------------------------------------------------------
Mesh::~Mesh() {
    SafeReleaseCom(vertex_buffer_);
    SafeReleaseCom(index_buffer_);
}

//-----------------------------------------------------------------------------
bool Mesh::LoadVertices(SDL_RWops* file) {
    SARE_ASSERT(file);

    uint32_t vertex_count = 0;
    if (!Uint32_Load(vertex_count, file)) {
        return false;
    }
    //printf("    vertex_count: %d\n", vertex_count);

    if (!vertices_.empty()) {
        std::vector<Vertex>(vertex_count).swap(vertices_);
        vertices_.clear();
    }
    if (!vertex_count) {
        return true;
    }

    vertices_.resize(vertex_count);
    if (!VertexArray_Load(&vertices_[0], vertex_count, file)) {
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool Mesh::LoadIndices(SDL_RWops* file) {
    SARE_ASSERT(file);

    uint32_t index_count = 0;
    if (!Uint32_Load(index_count, file)) {
        return false;
    }
    //printf("    index_count: %d\n", index_count);

    if (!indices_.empty()) {
        std::vector<uint16_t>(index_count).swap(indices_);
        indices_.clear();
    }
    if (!index_count) {
        return true;
    }

    indices_.resize(index_count);
    if (!Uint16Array_Load(&indices_[0], index_count, file)) {
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool Mesh::LoadSubMeshes(SDL_RWops* file) {
    SARE_ASSERT(file);

    uint32_t sub_mesh_count = 0;
    if (!Uint32_Load(sub_mesh_count, file)) {
        return false;
    }
    //printf("    submesh_count: %d\n", submesh_count);

    if (!sub_meshes_.empty()) {
        std::vector<SubMesh>(sub_mesh_count).swap(sub_meshes_);
        sub_meshes_.clear();
    }
    if (!sub_mesh_count) {
        return true;
    }

    sub_meshes_.resize(sub_mesh_count);
    for (uint32_t i = 0; i < sub_meshes_.size(); ++i) {
        if (!sub_meshes_[i].Load(file)) {
            return false;
        }
    }

    UpdateVertexRanges();
    return true;
}

//-----------------------------------------------------------------------------
void Mesh::UpdateVertexRanges() {
    for (uint32_t sm_i = 0; sm_i < sub_meshes_.size(); ++sm_i) {
        uint32_t first_index = sub_meshes_[sm_i].first_index();
        uint32_t next_first_index = 0;

        if (sm_i + 1 < sub_meshes_.size()) {
            next_first_index = sub_meshes_[sm_i + 1].first_index();
        }
        else {
            next_first_index = indices_.size();
        }

        uint32_t first_vertex = vertices_.size();
        uint32_t max_vertex = 0;

        for (uint32_t i = first_index; i < next_first_index; ++i) {
            if (first_vertex > indices_[i]) {
                first_vertex = indices_[i];
            }
            if (max_vertex < indices_[i]) {
                max_vertex = indices_[i];
            }
        }
        sub_meshes_[sm_i].SetVertexRange(first_vertex, max_vertex - first_vertex + 1);
    }
}

//-----------------------------------------------------------------------------
void Mesh::Dump() {
    Logger& logger = Logger::instance();
    LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Info);
    logger << "Mesh dump\n";
    logger.FixIndent();
    logger.IncIndent();
    logger << "vertex count: " << (uint32_t)vertices_.size() << ";\n";
    logger << "index count: " << (uint32_t)indices_.size() << ";\n";
    logger << "submesh count: " << (uint32_t)sub_meshes_.size() << ";\n";
    logger.FixIndent();
    logger.IncIndent();
    for (uint32_t sm_i = 0; sm_i < sub_meshes_.size(); ++sm_i) {
        logger << "v=[" << sub_meshes_[sm_i].first_vertex() << ", " << sub_meshes_[sm_i].vertex_count() << "];\n";
        logger << "i=[" << sub_meshes_[sm_i].first_index() << ", " << sub_meshes_[sm_i].primitive_count() << "];\n";
    }
    logger.RestoreIndent();
    logger.RestoreIndent();
}

//-----------------------------------------------------------------------------
bool Mesh::MapToDevice() {

    SARE_ASSERT(vertices_.size());
    SARE_ASSERT(indices_.size());
    SARE_ASSERT_MSG(!vertex_buffer_, "already mapped");
    SARE_ASSERT_MSG(!index_buffer_, "already mapped");

    HRESULT hr = Engine::instance().direct3ddevice9()->CreateVertexBuffer(
        vertices_.size() * sizeof(Vertex),
        D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vertex_buffer_, nullptr);
    if (D3DERR_OUTOFVIDEOMEMORY == hr) {
        hr = Engine::instance().direct3ddevice9()->CreateVertexBuffer(
            vertices_.size() * sizeof(Vertex),
            D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &vertex_buffer_, nullptr);
        if (E_OUTOFMEMORY == hr) {
            Engine::instance().HandleError(kErrorCode_NotEnoughMemory, kErrorLevel_Fatal);
            return false;
        }
    }
    else {
        SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");
        hardware_vb_ = true;
    }

    void* vertices = nullptr;
    hr = vertex_buffer_->Lock(0, 0, &vertices, 0);
    SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");
    memcpy(vertices, &vertices_[0], vertices_.size() * sizeof(Vertex));
    hr = vertex_buffer_->Unlock();
    SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");

    // indices
    hr = Engine::instance().direct3ddevice9()->CreateIndexBuffer(
        indices_.size() * sizeof(uint16_t),
        D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &index_buffer_, nullptr);
    if (D3DERR_OUTOFVIDEOMEMORY == hr) {
        hr = Engine::instance().direct3ddevice9()->CreateIndexBuffer(
            indices_.size() * sizeof(uint16_t),
            D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &index_buffer_, nullptr);
        if (E_OUTOFMEMORY == hr) {
            Engine::instance().HandleError(kErrorCode_NotEnoughMemory, kErrorLevel_Fatal);
            return false;
        }
    }
    else {
        SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");
        hardware_ib_ = true;
    }

    void* indices = nullptr;
    hr = index_buffer_->Lock(0, 0, &indices, 0);
    SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");
    memcpy(indices, &indices_[0], indices_.size() * sizeof(uint16_t));
    hr = index_buffer_->Unlock();
    SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");

    if (s_vertex_declaration) {
        return true;
    }
    else {
        D3DVERTEXELEMENT9 vertex_elemants[] = {
            {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
            {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
            {0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
            {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
            D3DDECL_END()
        };
        hr = Engine::instance().direct3ddevice9()->CreateVertexDeclaration(&vertex_elemants[0],
                                                                    &s_vertex_declaration);
        SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Mesh::MapToDeviceTextures(const char* directory) {
    for (uint32_t submesh_index = 0; submesh_index < sub_meshes_.size(); ++submesh_index) {
        if (!sub_meshes_[submesh_index].diffuse_map().MapToDevice(directory)) {
            return false;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Mesh::SetAsCurrentInDevice() {
    SARE_ASSERT_MSG(vertex_buffer_, "vb not mapped");
    SARE_ASSERT_MSG(index_buffer_, "ib not mapped");
    HRESULT hr = Engine::instance().direct3ddevice9()->SetStreamSource(0, vertex_buffer_, 0, sizeof(Vertex));
    SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");
    hr = Engine::instance().direct3ddevice9()->SetIndices(index_buffer_);
    SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");
    hr = Engine::instance().direct3ddevice9()->SetVertexDeclaration(s_vertex_declaration);
    SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");
    return true;
}

//-----------------------------------------------------------------------------
bool Mesh::Render() {
    SARE_ASSERT_MSG(vertex_buffer_, "vb not mapped");
    SARE_ASSERT_MSG(index_buffer_, "ib not mapped");
    for (uint32_t submesh_index = 0; submesh_index < sub_meshes_.size(); ++submesh_index) {
        sub_meshes_[submesh_index].Render();
    }

    return true;
}

//-----------------------------------------------------------------------------
bool Mesh::CreateCube(const D3dxVector3Ex& size) {
    D3dxVector3Ex points[8] = {
        D3dxVector3Ex(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z),
        D3dxVector3Ex(-0.5f * size.x, 0.5f  * size.y, -0.5f * size.z),
        D3dxVector3Ex(0.5f  * size.x, 0.5f  * size.y, -0.5f * size.z),
        D3dxVector3Ex(0.5f  * size.x, -0.5f * size.y, -0.5f * size.z),
        D3dxVector3Ex(-0.5f * size.x, -0.5f * size.y, 0.5f  * size.z),
        D3dxVector3Ex(-0.5f * size.x, 0.5f  * size.y, 0.5f  * size.z),
        D3dxVector3Ex(0.5f  * size.x, 0.5f  * size.y, 0.5f  * size.z),
        D3dxVector3Ex(0.5f  * size.x, -0.5f * size.y, 0.5f  * size.z)
    };
    vertices_.resize(24);
#if 0
    // front side
    vertices_[0].position = points[0];
    vertices_[0].normal = D3dxVector3Ex(0, 0, -1);
    vertices_[1].position = points[1];
    vertices_[1].normal = D3dxVector3Ex(0, 0, -1);
    vertices_[2].position = points[2];
    vertices_[2].normal = D3dxVector3Ex(0, 0, -1);
    vertices_[3].position = points[3];
    vertices_[3].normal = D3dxVector3Ex(0, 0, -1);

    // back side
    vertices_[4].position = points[7];
    vertices_[4].normal = D3dxVector3Ex(0, 0, 1);
    vertices_[5].position = points[6];
    vertices_[5].normal = D3dxVector3Ex(0, 0, 1);
    vertices_[6].position = points[5];
    vertices_[6].normal = D3dxVector3Ex(0, 0, 1);
    vertices_[7].position = points[4];
    vertices_[7].normal = D3dxVector3Ex(0, 0, 1);

    // left side
    vertices_[8].position = points[4];
    vertices_[8].normal = D3dxVector3Ex(-1, 0, 0);
    vertices_[9].position = points[5];
    vertices_[9].normal = D3dxVector3Ex(-1, 0, 0);
    vertices_[10].position = points[1];
    vertices_[10].normal = D3dxVector3Ex(-1, 0, 0);
    vertices_[11].position = points[0];
    vertices_[11].normal = D3dxVector3Ex(-1, 0, 0);

    // right side
    vertices_[12].position = points[3];
    vertices_[12].normal = D3dxVector3Ex(1, 0, 0);
    vertices_[13].position = points[2];
    vertices_[13].normal = D3dxVector3Ex(1, 0, 0);
    vertices_[14].position = points[6];
    vertices_[14].normal = D3dxVector3Ex(1, 0, 0);
    vertices_[15].position = points[7];
    vertices_[15].normal = D3dxVector3Ex(1, 0, 0);

    // bottom side
    vertices_[16].position = points[4];
    vertices_[16].normal = D3dxVector3Ex(0, -1, 0);
    vertices_[17].position = points[0];
    vertices_[17].normal = D3dxVector3Ex(0, -1, 0);
    vertices_[18].position = points[3];
    vertices_[18].normal = D3dxVector3Ex(0, -1, 0);
    vertices_[19].position = points[7];
    vertices_[19].normal = D3dxVector3Ex(0, -1, 0);

    // top side
    vertices_[20].position = points[1];
    vertices_[20].normal = D3dxVector3Ex(0, 1, 0);
    vertices_[21].position = points[5];
    vertices_[21].normal = D3dxVector3Ex(0, 1, 0);
    vertices_[22].position = points[6];
    vertices_[22].normal = D3dxVector3Ex(0, 1, 0);
    vertices_[23].position = points[2];
    vertices_[23].normal = D3dxVector3Ex(0, 1, 0);
#endif
    indices_.resize(36);
    // front side
    indices_[0] = 0;
    indices_[1] = 1;
    indices_[2] = 2;
    indices_[3] = 0;
    indices_[4] = 2;
    indices_[5] = 3;
    // back side
    indices_[6] = 7;
    indices_[7] = 6;
    indices_[8] = 5;
    indices_[9] = 7;
    indices_[10] = 5;
    indices_[11] = 4;
    // left side
    indices_[12] = 8;
    indices_[13] = 9;
    indices_[14] = 10;
    indices_[15] = 8;
    indices_[16] = 10;
    indices_[17] = 11;
    // right side
    indices_[18] = 12;
    indices_[19] = 13;
    indices_[20] = 14;
    indices_[21] = 12;
    indices_[22] = 14;
    indices_[23] = 15;
    // bottom side
    indices_[24] = 16;
    indices_[25] = 17;
    indices_[26] = 18;
    indices_[27] = 16;
    indices_[28] = 18;
    indices_[29] = 19;
    // top side
    indices_[30] = 20;
    indices_[31] = 21;
    indices_[32] = 22;
    indices_[33] = 20;
    indices_[34] = 22;
    indices_[35] = 23;

    MapToDevice();
    sub_meshes_.push_back(SubMesh(0, 24, 0, 36));
    return true;
}


//-----------------------------------------------------------------------------
bool Mesh::SaveToX(const char* file_name)
{
    LPD3DXMESH pD3DXMesh{};
    D3DVERTEXELEMENT9 vertex_elemants[] =
    {
        {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        {0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
        D3DDECL_END()
    };

    HRESULT  hr = D3DXCreateMesh(indices_.size() / 3,
                                vertices_.size(),
                                D3DXMESH_SYSTEMMEM | D3DXMESH_MANAGED,
                                &vertex_elemants[0],
                                Engine::instance().direct3ddevice9(),
                                &pD3DXMesh);

    if (FAILED(hr))
    {
        return false;
    }

    std::shared_ptr<ID3DXMesh> spD3DXMesh(pD3DXMesh,
        [](auto p)
        {
            if (p){p->Release();}
        });

    // vertices
    Vertex* vertices{};
    spD3DXMesh->LockVertexBuffer(0, (void**)&vertices);
    for (uint16_t vi = 0; vi < vertices_.size(); ++vi)
    {
        vertices[vi] = vertices_[vi];
    }
    spD3DXMesh->UnlockVertexBuffer();

    // indices
    Index* indices{};
    spD3DXMesh->LockIndexBuffer(0, (void**)&indices);
    for (uint16_t ii = 0; ii < indices_.size(); ++ii)
    {
        indices[ii] = indices_[ii];
    }
    spD3DXMesh->UnlockIndexBuffer();

    // attributes
    std::vector<D3DXATTRIBUTERANGE> attributeTable;
    for (uint32_t sm_i = 0; sm_i < sub_meshes_.size(); ++sm_i)
    {
        SubMesh& subMesh = sub_meshes_[sm_i];
        D3DXATTRIBUTERANGE attributeRange = {};
        attributeRange.AttribId = sm_i;
        attributeRange.VertexStart = subMesh.first_vertex();
        attributeRange.VertexCount = subMesh.vertex_count();
        attributeRange.FaceStart = subMesh.first_index() / 3;
        attributeRange.FaceCount = subMesh.primitive_count();
        attributeTable.push_back(attributeRange);
    }

    if (!attributeTable.empty())
    {
        spD3DXMesh->SetAttributeTable(&attributeTable[0], attributeTable.size());
    }

    std::vector<D3DXMATERIAL> materials;
    for (uint32_t sm_i = 0; sm_i < sub_meshes_.size(); ++sm_i)
    {
        D3DCOLORVALUE color = { 1.0f, 1.0f, 1.0f, 1.0f };
        SubMesh& subMesh = sub_meshes_[sm_i];
        D3DXMATERIAL material = {};
        material.MatD3D.Diffuse = color;
        material.MatD3D.Ambient = color;
        material.pTextureFilename = (char*)subMesh.diffuse_map().fileName();
        materials.push_back(material);
    }

    hr = D3DXSaveMeshToX(file_name,
        spD3DXMesh.get(),
        0,
        &materials[0],
        0,
        materials.size(),
        D3DXF_FILEFORMAT_TEXT | D3DXF_FILESAVE_TOFILE);

    return SUCCEEDED(hr);

}
} // end of sare
