#include "rotator.h"

namespace sare {

//-----------------------------------------------------------------------------
Rotator::Rotator() {
    orts_[kAxis_X].Set(1.0f, 0.0f, 0.0f);
    orts_[kAxis_Y].Set(0.0f, 1.0f, 0.0f);
    orts_[kAxis_Z].Set(0.0f, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
void Rotator::Frame() {
    D3DXMATRIX rotation_matrix;
    D3DXVECTOR3 tmp;
    D3DXMatrixRotationYawPitchRoll(&rotation_matrix, D3DXToRadian(pyr_.y),
                                   D3DXToRadian(pyr_.x), D3DXToRadian(pyr_.z));
    tmp = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    D3DXVec3TransformNormal(&orts_[kAxis_Z], &tmp, &rotation_matrix);
    tmp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXVec3TransformNormal(&orts_[kAxis_Y], &tmp, &rotation_matrix);
    tmp = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
    D3DXVec3TransformNormal(&orts_[kAxis_X], &tmp, &rotation_matrix);
    for (uint32_t i = 0; i < kAxis_Count; ++i) {
        D3DXVec3Normalize(&tmp, &orts_[i]);
        orts_[i] = tmp;
    }
}

} // end of sare
