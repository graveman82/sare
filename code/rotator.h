#ifndef SARE_ROTATOT_H_
#define SARE_ROTATOT_H_


#include "math_vectors.h"
#include "shared_constants.h"
#include <stdint.h>

namespace sare {

//-----------------------------------------------------------------------------

class Rotator {
public:
    Rotator();
    void Frame();

    const D3dxVector3Ex& ort(Axis axis) const { return orts_[axis]; }
    D3dxVector3Ex& pyr() { return pyr_; }
    const D3dxVector3Ex& pyr() const { return pyr_; }

protected:
    D3dxVector3Ex pyr_; // pitch, yaw, roll
    D3dxVector3Ex orts_[kAxis_Count];
};

} // end of sare

#endif // SARE_ROTATOT_H_

