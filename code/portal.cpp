#include "portal.h"
#include "integers.h"
#include "assert.h"

namespace sare
{
//-----------------------------------------------------------------------------
//
bool Portal::BuildFrom( const WorldShape& world_shape )
{
    const auto& static_mesh = world_shape.static_mesh();
    const auto& vertices = static_mesh.vertices();
    const auto& indices = static_mesh.indices();

    const UCount16 n_tris = static_mesh.TriCount();
    const UCount16 n_tri_vertex = 3;

    auto FindEdge = [&indices, n_tris] (UIndex16 uc_tri_i, UIndex16 e_v_i1, UIndex16 e_v_i2) -> bool // uc - under consideration
    {
        for ( UIndex16 tri_i = 0; tri_i < n_tris; ++tri_i )
        {
            if ( uc_tri_i == tri_i ) // don't test triangle with itself
                continue;

            for ( UIndex16 point_i = 0; point_i < n_tri_vertex; ++point_i )
            {
                UIndex16 i1 = indices[ 3 * tri_i + point_i ];
                UIndex16 i2 = indices[ 3 * tri_i + ( point_i + 1 ) % 3 ];

                bool fAdjEdgeFound { false };

                if ( e_v_i1 == i1 && e_v_i2 == i2 )
                {
                    return true;
                }
                if ( e_v_i1 == i2 && e_v_i2 == i1 )
                {
                    return true;
                }
            }
        }

        return false;
    };

    for ( UIndex16 tri_i = 0; tri_i < n_tris; ++tri_i )
    {
        for ( UIndex16 point_i = 0; point_i < n_tri_vertex; ++point_i )
        {
            UIndex16 i1 = indices[ 3 * tri_i + point_i ];
            UIndex16 i2 = indices[ 3 * tri_i + ( point_i + 1 ) % 3 ];

            if ( !FindEdge( tri_i, i1, i2 ) )
            {
                convex_polygons_[0].Append() = vertices[i1].position;
            }
        }
    }

    return true;
}
//-----------------------------------------------------------------------------
//
void Portal::FixLoop()
{

}
//
} // end of sare

