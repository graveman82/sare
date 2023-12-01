#include "worldshape.h"
#include "assert.h"
#include "log.h"
#include "engine.h"
#include "file_util.h"

#include <conio.h>

namespace sare {

//-----------------------------------------------------------------------------
static const uint32_t k_ssm_id[2] = {0xDEAFBABE, 0};

//-----------------------------------------------------------------------------
bool WorldShape::Load(SDL_RWops* file) {
    SARE_ASSERT(file);

    // ID
    uint32_t id;
    if (!Uint32_Load(id, file)) {
        return false;
    }
    //printf("  mesh id: %x\n", id);
    if (id != k_ssm_id[0]) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "Invalid object id(" << id << ")!\n";
        Engine::instance().HandleError(kErrorCode_MalformedData, kErrorLevel_Fatal);
        return false;
    }

    // desc
    static std::string desc;
    desc.clear();
    if (!StdString_Load(desc, file)) {
        return false;
    }
    //printf("    desc: %s\n", desc.c_str());

    Rename(desc.c_str());
    if (desc.find("zone") != desc.npos) {
        printf("desc: %s\n", desc.c_str());
        SetBoolProperty("visibility", false);
    }
    if (desc.find("vollight") != desc.npos) {
        printf("desc: %s\n", desc.c_str());
        SetBoolProperty("visibility", false);
    }
    if (desc.find("volfog") != desc.npos) {
        printf("desc: %s\n", desc.c_str());
        SetBoolProperty("visibility", false);
    }
    if (desc.find("glass") != desc.npos) {
        printf("desc: %s\n", desc.c_str());
        SetBoolProperty("visibility", false);
    }
    if (desc.find("barrier") != desc.npos) {
        printf("desc: %s\n", desc.c_str());
        SetBoolProperty("visibility", false);
    }
    if (desc.find("actgrp") != desc.npos) {
        printf("desc: %s\n", desc.c_str());
        SetBoolProperty("visibility", false);
    }
    if (desc.find("phys") != desc.npos) {
        printf("desc: %s\n", desc.c_str());
        SetBoolProperty("visibility", false);
    }
    if (desc.find("portal") != desc.npos) {
        printf("desc: %s\n", desc.c_str());
        SetBoolProperty("visibility", false);
    }
    if (desc.find("antyp") != desc.npos) {
        printf("desc: %s\n", desc.c_str());
        SetBoolProperty("visibility", false);
    }



    // world TM
    if (!Matrix4x4_Load(world_tm_, file)) {
        return false;
    }
    //printf("    world TM:\n");
    //Matrix4x4_Printf(worldTM_, "      ");

    // unk uint32_t
    uint32_t u32unk0;
    if (!Uint32_Load(u32unk0, file)) {
        return false;
    }
    if (u32unk0 != 2 && u32unk0 != 1) printf("    u32unk0: %x\n", u32unk0);

    // Vertices
    if (!static_mesh_.LoadVertices(file)) {
        return false;
    }

    // Uvws
    if (!static_mesh_.LoadUvws(file)) {
        return false;
    }

    // Bound box
    if (!Vector3D_Load(boundbox_.pt_min, file)) {
        return false;
    }
    if (!Vector3D_Load(boundbox_.pt_max, file)) {
        return false;
    }

    // Indices
    if (!static_mesh_.LoadIndices(file)) {
        return false;
    }

    // Submeshes
    if (!static_mesh_.LoadSubMeshes(file)) {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
void WorldShape::Draw()
{
    auto Visible = [this]() -> bool
    {
        bool visible = true;
        SARE_ASSERT_MSG(GetBoolProperty("visibility", visible), "property with given name is not exist");
        GetBoolProperty("visibility", visible);
        return visible;;
    };
    if (!Visible())
    {
        return;
    }

    Engine::instance().direct3ddevice9()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    Engine::instance().direct3ddevice9()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    Engine::instance().direct3ddevice9()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    static std::string desc;
    desc = name_;

    if (desc.find("phys") != desc.npos) {
        Engine::instance().direct3ddevice9()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    }
    if (desc.find("barrier") != desc.npos) {
        Engine::instance().direct3ddevice9()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        //return;
    }
    else {
        Engine::instance().direct3ddevice9()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        //return;
    }
    static_mesh_.SetAsCurrentInDevice();
    static_mesh_.Render();
}

//-----------------------------------------------------------------------------
bool WorldShape::SaveToX(const char* dir_name)
{
    auto Visible = [this]() -> bool
    {
        bool visible = true;
        SARE_ASSERT_MSG(GetBoolProperty("visibility", visible), "property with given name is not exist");
        GetBoolProperty("visibility", visible);
        return visible;;
    };
    if (!Visible())
    {
        return true;
    }

    std::string file_name = dir_name;
    file_name.append("/");
    std::string shape_name;
    for (const auto& c : name_)
    {
        if (c == '|')
        {
            shape_name.append("_OR_");
        }
        else
        {
            shape_name += c;
        }
    }
    file_name.append(shape_name);
    file_name.append(".x");

    printf("-- saving %s... ", file_name.c_str());
    bool result = static_mesh_.SaveToX(file_name.c_str());
    printf("%s\n", result ? "done" : "failed");
    if (!result)
    {
        getche();
    }
    return result;
}
} // end of sare
