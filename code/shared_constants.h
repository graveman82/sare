#ifndef SARE_SHARED_CONSTANTS_H_
#define SARE_SHARED_CONSTANTS_H_

namespace sare {

//-----------------------------------------------------------------------------
enum Axis {
    kAxis_X, kAxis_Y, kAxis_Z, kAxis_Count
};

//-----------------------------------------------------------------------------

enum Direction {
    kDirection_Front,
    kDirection_Back,
    kDirection_Right,
    kDirection_Left,
    kDirection_Up,
    kDirection_Down,
    kDirection_Count
};

//-----------------------------------------------------------------------------
enum ContentType {
    kContentType_Shared,
    kContentType_Level
};

} // end of sare

#endif // SARE_SHARED_CONSTANTS_H_

