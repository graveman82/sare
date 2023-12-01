#ifndef SARE_WORLD_H_
#define SARE_WORLD_H_

#include <vector>
#include <memory>
#include "worldshape.h"
#include "shared_constants.h"
#include "portal.h"

namespace sare {

class World {
public:
    using WorldShapeSP = std::shared_ptr<WorldShape>;
    using WorldShapeSPVector = std::vector<WorldShapeSP>;

    using PortalSP = std::shared_ptr<Portal>;
    using PortalSPVector = std::vector<PortalSP>;

    World();
    ~World();
    bool Load(const char* level_name);
    bool SaveToX (const char* dir_name);
    void Draw();

private:
    void Clear();
    bool LoadWorldShapes();
    bool MapToDeviceWorldShapes();
    void DrawWorldShapes();

    std::string level_name_;
    WorldShapeSPVector world_shapes_;
    PortalSPVector portals_;
};

} // end of sare
#endif // SARE_WORLD_H_

