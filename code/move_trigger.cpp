#include "move_trigger.h"
#include "engine.h"

namespace sare {


//-----------------------------------------------------------------------------
MoveTrigger::MoveTrigger() :
    position_(nullptr),
    direction_(nullptr),
    speed_(0.0f) {

}

//-----------------------------------------------------------------------------
void MoveTrigger::Frame() {
    if (!on_)
        return;
    float dt = Engine::instance().update_period();
    SARE_ASSERT(position_);
    SARE_ASSERT(direction_);
    D3dxVector3Ex normalized_direction;

    (*position_) += dt * speed_ * (*D3DXVec3Normalize(&normalized_direction, direction_));
}

} // end of sare
