#include "scene_node.h"
#include "shared_functions.h"

namespace sare {

void SceneNode::MoveAlongX(float dx) {
    position_.x += dx;
    flags_.set(kFlagBit_DirtyLocalTm);
}

void SceneNode::MoveAlongY(float dy) {
    position_.y += dy;
    flags_.set(kFlagBit_DirtyLocalTm);
}

void SceneNode::MoveAlongZ(float dz) {
    position_.z += dz;
    flags_.set(kFlagBit_DirtyLocalTm);
}

void SceneNode::MoveAlongLocalX(float dx) {
    const D3dxVector3Ex& local_x = rotator_.ort(kAxis_X);
    position_ += local_x * dx;
    flags_.set(kFlagBit_DirtyLocalTm);
}

void SceneNode::MoveAlongLocalY(float dy) {
    const D3dxVector3Ex& local_y = rotator_.ort(kAxis_Y);
    position_ += local_y * dy;
    flags_.set(kFlagBit_DirtyLocalTm);
}

void SceneNode::MoveAlongLocalZ(float dz) {
    const D3dxVector3Ex& local_z = rotator_.ort(kAxis_Z);
    position_ += local_z * dz;
    flags_.set(kFlagBit_DirtyLocalTm);
}

void SceneNode::SetPosition(float x, float y, float z) {
    position_.Set(x, y, z);
    flags_.set(kFlagBit_DirtyLocalTm);
}

void SceneNode::SetScale(float x, float y, float z) {
    scale_.Set(x, y, z);
    flags_.set(kFlagBit_DirtyLocalTm);
}

void SceneNode::SetRotation(float p, float y, float r) {
    p = Clamp(p, -89.5f, 89.5f);
    y = ClampAngleInDegree(y);
    r = ClampAngleInDegree(r);
    rotator_.pyr().Set(p, y, r);
    flags_.set(kFlagBit_DirtyLocalTm);
}

void SceneNode::Rotate(float dp, float dy, float dr) {
    rotator_.pyr() += D3dxVector3Ex(dp, dy, dr);
    rotator_.pyr().x = Clamp(rotator_.pyr().x, -89.5f, 89.5f);
    rotator_.pyr().y = ClampAngleInDegree(rotator_.pyr().y);
    rotator_.pyr().z = ClampAngleInDegree(rotator_.pyr().z);
    flags_.set(kFlagBit_DirtyLocalTm);
}

//-----------------------------------------------------------------------------
const D3DXMATRIX& SceneNode::LocalMatrix() {
    static D3DXMATRIX mTmp;

    if (flags_.test(kFlagBit_DirtyLocalTm)) {
        D3DXMatrixMultiply(&local_tm_, D3DXMatrixScaling(&local_tm_, scale_.x, scale_.y, scale_.z),
                           D3DXMatrixRotationYawPitchRoll(&mTmp, D3DXToRadian(rotator_.pyr().y),
                                                          D3DXToRadian(rotator_.pyr().x),
                                                          D3DXToRadian(rotator_.pyr().z)));
        D3DXMatrixMultiply(&local_tm_, &local_tm_, D3DXMatrixTranslation(&mTmp, position_.x, position_.y, position_.z));
        flags_.reset(kFlagBit_DirtyLocalTm);
    }

    return local_tm_;
}
} // end of sare
