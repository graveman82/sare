#ifndef SARE_CONVEX_POLYGON_H_
#define SARE_CONVEX_POLYGON_H_

#include "math_vectors.h"
#include "integers.h"
#include <vector>

namespace sare
{
//-----------------------------------------------------------------------------
//
class ConvexPolygon
{
public:
    using Vertex = D3dxVector3Ex;
    using VertexVector = std::vector<Vertex>;
    typedef D3dxVector3Ex Vector;

    enum Order{k_ccw, k_cw};

    ConvexPolygon() = default;

    bool Reserve( UCount32 n );

    UCount32 Count() const { return vertices_.size(); }

    Vertex& Append();

    void FixOrder(Order order);
private:
    VertexVector vertices_;

    Order order{k_ccw};
};

} // end of sare
#endif // SARE_CONVEX_POLYGON_H_
