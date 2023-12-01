#include "world.h"

#include <sdl.h>
#include <string>
#include "assert.h"
#include "log.h"
#include "engine.h"
#include "file_util.h"

namespace sare {

static const uint32_t k_spf_id[2] = {0xDEADBEEF, 0};
const uint32_t k_max_worldshape_count = 4000;

const char* k_maps_directory = "../Data/Maps/";
const char* k_worldshapes_textures_directory = "../Data/Textures/Levels/";
static std::string s_filepath;

//-----------------------------------------------------------------------------
World::World() {

}

//-----------------------------------------------------------------------------
World::~World() {
    Clear();
}

//-----------------------------------------------------------------------------
void World::Clear() {
    world_shapes_.clear();
}

//-----------------------------------------------------------------------------
bool World::Load(const char* level_name) {
    level_name_ = level_name;
    if (!LoadWorldShapes()) {
        return false;
    }
    if (!MapToDeviceWorldShapes()) {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool World::SaveToX(const char* dir_name)
{

    for (uint32_t ws_i = 0; ws_i < world_shapes_.size(); ++ws_i)
    {
        WorldShapeSP spWorldShape = world_shapes_[ws_i];
        spWorldShape->SaveToX(dir_name);
    }


    return true;
}

//-----------------------------------------------------------------------------
void World::Draw() {
    DrawWorldShapes();
}

//-----------------------------------------------------------------------------
bool World::LoadWorldShapes() {
    Clear();
    SARE_ASSERT_MSG(!level_name_.empty(), "No map file name");

    s_filepath = k_maps_directory;
    s_filepath += level_name_;
    s_filepath += ".mpk";
    SDL_RWops* file = SDL_RWFromFile(s_filepath.c_str(), "rb");
    if (!file) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWFromFile() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        return false;
    }

    uint32_t file_size = (uint32_t) SDL_RWsize(file);
    if (uint32_t(-1) == file_size) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWsize() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        SDL_RWclose(file);
        return false;
    }
    printf("  size of file: %u\n", file_size);

    if (12 > file_size) {
        Engine::instance().HandleError(kErrorCode_MalformedData, kErrorLevel_Fatal);
        SDL_RWclose(file);
        return false;
    }

    // ssm ID
    uint32_t spf_id;
    if (uint32_t(-1) == SDL_RWseek(file, -4,  RW_SEEK_END)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "SDL_RWseek() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        SDL_RWclose(file);
        return false;
    }
    if (!Uint32_Load(spf_id, file)) {
        SDL_RWclose(file);
        return false;
    }
    printf("  spf id: %x\n", spf_id);
    if (spf_id != k_spf_id[0]) {
        Engine::instance().HandleError(kErrorCode_MalformedData, kErrorLevel_Fatal);
        SDL_RWclose(file);
        return false;
    }

    // number of objects
    uint32_t object_count = 0;
    if (-1 == SDL_RWseek(file, -8,  RW_SEEK_END)) {
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        SDL_RWclose(file);
        return false;
    }
    if (!Uint32_Load(object_count, file)) {
        SDL_RWclose(file);
        return false;
    }

    if (object_count == 0 || object_count > k_max_worldshape_count) {
        Engine::instance().HandleError(kErrorCode_MalformedData, kErrorLevel_Fatal);
        SDL_RWclose(file);
        return false;
    }
    printf("  object_count: %d\n", object_count);
    // objects offsets
    std::vector<uint32_t> object_offsets;
    object_offsets.reserve(object_count);
    if (-1 == SDL_RWseek(file, file_size - 8 - 4 * object_count,  RW_SEEK_SET)) {
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
        SDL_RWclose(file);
        return false;
    }

    for (uint32_t i = 0; i < object_count; ++i) {
        uint32_t offs;
        if (!Uint32_Load(offs, file)) {
            SDL_RWclose(file);
            return false;
        }
        object_offsets.push_back(offs);
    }
    world_shapes_.reserve(object_count);
    for ( uint32_t i = 0; i < object_offsets.size(); ++i )
    {
        if ( -1 == SDL_RWseek(file, object_offsets[i],  RW_SEEK_SET) )
        {
            Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal, kExternalApi_Sdl2);
            SDL_RWclose(file);
            return false;
        }

        WorldShapeSP world_shape { new WorldShape() };

        if ( !world_shape->Load(file) )
        {
            SDL_RWclose(file);
            return false;
        }
        //if (45 == i) {
            //world_shape->static_mesh().Dump();
        //}
        printf("%u object\n", i);
        //printf("Press any key to load next shape...\n"); getch();
        if ( world_shape->NameContains("portal") )
        {
            PortalSP portal { new Portal() };

            portal->BuildFrom( *world_shape );
            portals_.push_back( portal );
        }
        else
        {
            world_shapes_.push_back( WorldShapeSP(world_shape) );
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
bool World::MapToDeviceWorldShapes() {
    s_filepath = k_worldshapes_textures_directory;
    s_filepath += level_name_;
    s_filepath += "/";
    for (uint32_t i = 0; i < world_shapes_.size(); ++i) {
        WorldShape& world_shape = *world_shapes_[i];
        if (!world_shape.static_mesh().MapToDevice()) {
            return false;
        }
        if (!world_shape.static_mesh().MapToDeviceTextures(s_filepath.c_str())) {
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
void World::DrawWorldShapes() {
    //world_shapes_[0]->Draw();return;
    for (uint32_t i = 0; i < world_shapes_.size(); ++i) {
        WorldShape& world_shape = *world_shapes_[i];
        world_shape.Draw();
    }
}
} // end of sare
