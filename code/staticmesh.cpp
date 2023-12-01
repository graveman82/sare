#include "staticmesh.h"
#include "file_util.h"
#include "assert.h"
#include "engine.h"
#include "log.h"
#include "shared_functions.h"

namespace sare {


//-----------------------------------------------------------------------------
StaticMesh::StaticMesh() :
    uvw_buffer_(nullptr),
    hardware_uvw_(false) {
}

//-----------------------------------------------------------------------------
StaticMesh::~StaticMesh() {
    SafeReleaseCom(uvw_buffer_);
}

//-----------------------------------------------------------------------------
bool StaticMesh::LoadUvws(SDL_RWops* file) {
    SARE_ASSERT(file);

    uint32_t uvw_count = 0;
    if (!Uint32_Load(uvw_count, file)) {
        return false;
    }
    //printf("    uvw_count: %d\n", uvw_count);

    if (!uvws_.empty()) {
        std::vector<D3dxVector3Ex>(uvw_count).swap(uvws_);
        uvws_.clear();
    }
    if (0 == uvw_count) {
        return true;
    }

    uvws_.resize(uvw_count);
    if (!D3dxVector3Ex_Load(&uvws_[0], uvw_count, file)) {
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool StaticMesh::MapToDevice() {
    if(!Mesh::MapToDevice()) {
        return false;
    }
    if(!uvws_.size())
        return true;
    SARE_ASSERT_MSG(!uvw_buffer_, "already mapped");

    HRESULT hr = Engine::instance().direct3ddevice9()->CreateVertexBuffer(
        uvws_.size() * sizeof(D3dxVector3Ex),
        D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &uvw_buffer_, nullptr);
    if (D3DERR_OUTOFVIDEOMEMORY == hr) {
        hr = Engine::instance().direct3ddevice9()->CreateVertexBuffer(
            uvws_.size() * sizeof(D3dxVector3Ex),
            D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &uvw_buffer_, nullptr);
        if (E_OUTOFMEMORY == hr) {
            Engine::instance().HandleError(kErrorCode_NotEnoughMemory, kErrorLevel_Fatal);
            return false;
        }
    }
    else {
        SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");
        hardware_uvw_ = true;
    }

    void* uvws = nullptr;
    hr = uvw_buffer_->Lock(0, 0, &uvws, 0);
    SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");
    memcpy(uvws, &uvws_[0], uvws_.size() * sizeof(D3dxVector3Ex));
    hr = uvw_buffer_->Unlock();
    SARE_ASSERT_MSG(D3D_OK == hr, "unexpected error code");

    return true;
}
} // end of sare
