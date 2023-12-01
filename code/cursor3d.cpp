#include "cursor3d.h"
#include "assert.h"
#include "log.h"
#include "engine.h"
#include "file_util.h"

#include <conio.h>

namespace sare {


//-----------------------------------------------------------------------------
bool Cursor3D::Init()
{

    SetBoolProperty("visibility", true);

    mesh_.CreateCube(D3dxVector3Ex(100, 100, 100));

    return true;
}

//-----------------------------------------------------------------------------
void Cursor3D::Draw()
{
    Engine::instance().direct3ddevice9()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    Engine::instance().direct3ddevice9()->SetTransform(D3DTS_WORLD, &world_tm());
    mesh_.SetAsCurrentInDevice();
    mesh_.Render();

}


} // end of sare

