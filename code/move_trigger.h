#ifndef SARE_MOVE_TRIGGER_H_
#define SARE_MOVE_TRIGGER_H_


#include "key_trigger.h"
#include "math_vectors.h"

namespace sare {

//-----------------------------------------------------------------------------
class MoveTrigger : public PressKeyTrigger {
public:
    MoveTrigger();
    virtual void Frame();

    void SetPosition(D3dxVector3Ex* position) { position_ = position; }
    void SetDirection(const D3dxVector3Ex* direction) { direction_ = direction; }
    void SetSpeed(float speed) { speed_ = speed; }

private:
    D3dxVector3Ex* position_;
    const D3dxVector3Ex* direction_;
    float speed_;
};

} // end of sare



#endif // SARE_MOVE_TRIGGER_H_
