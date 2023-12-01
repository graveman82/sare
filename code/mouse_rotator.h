#ifndef SARE_MOUSE_ROTATOR_H_
#define SARE_MOUSE_ROTATOR_H_

#include "rotator.h"

namespace sare {

class MouseRotator : public Rotator {
public:
    MouseRotator();
    void OnMouseMotion(int32_t delta_x, int32_t delta_y);
    void SetSensitivity(float sensitivity) { sensitivity_ = sensitivity; }
    float sensitivity() const { return sensitivity_; }
private:
    float sensitivity_;
};

} // end of sare
#endif // SARE_MOUSE_ROTATOR_H_

