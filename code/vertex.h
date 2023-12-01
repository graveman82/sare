#ifndef SARE_VERTEX_H_
#define SARE_VERTEX_H_

#include "math_vectors.h"

namespace sare {

//-----------------------------------------------------------------------------
/** @class Vertex Вершина.
*/
struct Vertex {
    D3dxVector3Ex position;
    uint32_t color;
    D3dxVector2Ex texcoords;
    D3dxVector2Ex texcoords2;
};

} // end of sare
#endif // SARE_VERTEX_H_

