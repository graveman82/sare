#include "convex_polygon.h"
#include <algorithm>

namespace sare
{
//-----------------------------------------------------------------------------
//
bool ConvexPolygon::Reserve( UCount32 n )
{
    vertices_.reserve(n);
    return true;
}

//-----------------------------------------------------------------------------
//
ConvexPolygon::Vertex& ConvexPolygon::Append()
{
    vertices_.push_back(Vertex());
    return vertices_.back();
}

//-----------------------------------------------------------------------------
//
void ConvexPolygon::FixOrder(Order order)
{
    if (Count() < 3)
        return;

    Vector v0v1 { vertices_[1] - vertices_[0] };
    D3DXVec3Normalize( &v0v1, &v0v1 );

    auto Pred = [this, &v0v1] ( const Vertex& a, const Vertex& b ) -> float
    {
        Vector v1a { a - vertices_[1] };
        D3DXVec3Normalize( &v1a, &v1a );
        Vector v1b { b - vertices_[1] };
        D3DXVec3Normalize( &v1b, &v1b );
        return Dot( v0v1, v1a ) > Dot( v0v1, v1b );
    };

    auto it = vertices_.begin();
    ++it;
    ++it;
    std::stable_sort( it, vertices_.end(), Pred );

}
//
} // end of sare
