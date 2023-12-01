#ifndef SARE_CAMERA_H
#define SARE_CAMERA_H

#include "move_trigger.h"
#include "mouse_rotator.h"
#include "shared_constants.h"

namespace sare {

//-----------------------------------------------------------------------------
/** @class
*/
class Camera {
public:
    Camera();

    void Frame();

    MouseRotator& mouse_rotator() { return mouse_rotator_; }
    const MouseRotator& mouse_rotator() const { return mouse_rotator_; }

    MoveTrigger& GetMoveTrigger(Direction direction) { return move_triggers_[direction]; }
    const MoveTrigger& GetMoveTrigger(Direction direction) const { return move_triggers_[direction]; }

    const D3dxVector3Ex& eye() const { return eye_; }
    D3DXMATRIX& view_matrix();
    D3DXMATRIX& projection_matrix();

private:
    MoveTrigger move_triggers_[kDirection_Count];
    MouseRotator mouse_rotator_;
    D3dxVector3Ex eye_;
    D3DXMATRIX view_matrix_;
    D3DXMATRIX proj_matrix_;
};

} // end of sare
#endif // SARE_CAMERA_H
