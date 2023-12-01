#ifndef SARE_BOUNDBOX_H_
#define SARE_BOUNDBOX_H_

#include "math_vectors.h"

namespace sare {

class BoundBox {
public:
    D3dxVector3Ex pt_min;
    D3dxVector3Ex pt_max;
};

} // end of sare
#endif // SARE_BOUNDBOX_H_

