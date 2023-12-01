#ifndef SARE_WORLDSHAPE_H_
#define SARE_WORLDSHAPE_H_

#include "scene_node.h"
#include "staticmesh.h"
#include "boundbox.h"

namespace sare
{

//-----------------------------------------------------------------------------
class WorldShape : public SceneNode
{
public:
    bool Load(SDL_RWops* file);
    bool SaveToX(const char* dir_name);
    void Draw();

    StaticMesh& static_mesh() { return static_mesh_; }
    const StaticMesh& static_mesh() const { return static_mesh_; }
private:
    StaticMesh static_mesh_;
    BoundBox boundbox_;
};

} // end of sare
#endif // SARE_WORLDSHAPE_H_

