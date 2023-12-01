#ifndef SARECURSOR3D_H_
#define SARECURSOR3D_H_

#include "scene_node.h"
#include "mesh.h"
#include "boundbox.h"

namespace sare
{

//-----------------------------------------------------------------------------
class Cursor3D : public SceneNode
{
public:
    bool Init();
    void Draw();

private:
    Mesh mesh_;
};

} // end of sare
#endif // SARECURSOR3D_H_
