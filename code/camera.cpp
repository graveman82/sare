#include "camera.h"

namespace sare {

//-----------------------------------------------------------------------------
Camera::Camera() {
    eye_ = D3dxVector3Ex(0.0f, 5.0f, -10.0f);
    for (uint32_t i = 0; i < kDirection_Count; ++i) {
        move_triggers_[i].SetPosition(&eye_);
    }
    move_triggers_[kDirection_Front].SetSpeed(0.001f);
    move_triggers_[kDirection_Back].SetSpeed(-0.001f);
    move_triggers_[kDirection_Right].SetSpeed(0.001f);
    move_triggers_[kDirection_Left].SetSpeed(-0.001f);
    move_triggers_[kDirection_Up].SetSpeed(0.001f);
    move_triggers_[kDirection_Down].SetSpeed(-0.001f);
}

//-----------------------------------------------------------------------------
void Camera::Frame() {
    mouse_rotator().Frame();
    move_triggers_[kDirection_Front].SetDirection(&mouse_rotator().ort(kAxis_Z));
    move_triggers_[kDirection_Back].SetDirection(&mouse_rotator().ort(kAxis_Z));
    move_triggers_[kDirection_Up].SetDirection(&mouse_rotator().ort(kAxis_Y));
    move_triggers_[kDirection_Down].SetDirection(&mouse_rotator().ort(kAxis_Y));
    move_triggers_[kDirection_Right].SetDirection(&mouse_rotator().ort(kAxis_X));
    move_triggers_[kDirection_Left].SetDirection(&mouse_rotator().ort(kAxis_X));
    for (uint32_t i = 0; i < kDirection_Count; ++i) {
        move_triggers_[i].Frame();
    }

}

//-----------------------------------------------------------------------------
D3DXMATRIX& Camera::view_matrix() {
    D3dxVector3Ex at = eye() + mouse_rotator().ort(kAxis_Z);
    D3dxVector3Ex up(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&view_matrix_, &eye(), &at, &up);
    return view_matrix_;
}

//-----------------------------------------------------------------------------
D3DXMATRIX& Camera::projection_matrix() {
    D3DXMatrixPerspectiveFovLH(&proj_matrix_, D3DXToRadian(45.0f), 4/3.0f, 0.1f, 1000.0f);
    return proj_matrix_;
}
} // end of sare
