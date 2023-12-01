#include "engine.h"

namespace sare {

//-----------------------------------------------------------------------------
bool Engine::InitializeWorld() {
    mesh_.reset(new Mesh());
    mesh_->CreateCube(D3dxVector3Ex(4, 2, 1));

    camera_.GetMoveTrigger(kDirection_Front).SetKeyCode(SDLK_w);
    camera_.GetMoveTrigger(kDirection_Front).SetSpeed(0.1f);

    camera_.GetMoveTrigger(kDirection_Back).SetKeyCode(SDLK_s);
    camera_.GetMoveTrigger(kDirection_Back).SetSpeed(-0.1f);

    camera_.GetMoveTrigger(kDirection_Right).SetKeyCode(SDLK_d);
    camera_.GetMoveTrigger(kDirection_Right).SetSpeed(0.1f);

    camera_.GetMoveTrigger(kDirection_Left).SetKeyCode(SDLK_a);
    camera_.GetMoveTrigger(kDirection_Left).SetSpeed(-0.1f);

    camera_.GetMoveTrigger(kDirection_Up).SetKeyCode(SDLK_z);
    camera_.GetMoveTrigger(kDirection_Up).SetSpeed(0.1f);

    camera_.GetMoveTrigger(kDirection_Down).SetKeyCode(SDLK_x);
    camera_.GetMoveTrigger(kDirection_Down).SetSpeed(-0.1f);


    camera_.mouse_rotator().SetSensitivity(0.1f);

    if (!world_.Load("3x02_Factory")) {
        return false;
    }

    /*if (!world_.SaveToX("D:/Documents/Projects/sare/sare/Data/Maps/exported_to_x"))
    {
        return false;
    }*/
    return true;
}

//-----------------------------------------------------------------------------
void Engine::TerminateWorld() {
    mesh_.reset();
}

//-----------------------------------------------------------------------------
bool Engine::DrawWorld() {
#if 0
    D3DXMATRIX& view_matrix = camera().view_matrix();
    direct3ddevice9()->SetTransform(D3DTS_VIEW, &view_matrix);
    D3DXMATRIX& proj_matrix = camera().projection_matrix();
    direct3ddevice9()->SetTransform(D3DTS_PROJECTION, &proj_matrix);
#else
    material_system().SetVertexShader("regular");
#endif
#if 0
    mesh_->SetAsCurrentInDevice();
    mesh_->Render();
#else
    Engine::instance().direct3ddevice9()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    world_.Draw();
#endif
    return true;
}
} // end of sare
