#ifndef SARE_PORTAL_H
#define SARE_PORTAL_H

#include "object.h"
#include "convex_polygon.h"
#include "worldshape.h"

namespace sare
{

class Portal : Object
{
public:
    bool BuildFrom( const WorldShape& world_shape );
    void FixLoop();
private:
    ConvexPolygon convex_polygons_[2];

};

//
} // end of sare

#endif // SARE_PORTAL_H

