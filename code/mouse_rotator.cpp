#include "mouse_rotator.h"

namespace sare {

//-----------------------------------------------------------------------------
MouseRotator::MouseRotator() : sensitivity_(0.0f) {

}

//-----------------------------------------------------------------------------
void MouseRotator::OnMouseMotion(int32_t delta_x, int32_t delta_y) {

    D3dxVector2Ex deltas(delta_x, delta_y);
    pyr_.y += sensitivity_ * deltas.x;
    while (pyr_.y < 0.0f) pyr_.y += 360.0f;
    while (pyr_.y > 360.0f) pyr_.y -= 360.0f;

    pyr_.x += sensitivity_ * deltas.y;
    if (pyr_.x > 89.0f) pyr_.x = 89.0f;
    else if (pyr_.x < -89.0f) pyr_.x = -89.0f;
}

} // end of sare
