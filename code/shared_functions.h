#ifndef SARE_SHARED_FUNCTIONS_H_
#define SARE_SHARED_FUNCTIONS_H_

namespace sare {

//-----------------------------------------------------------------------------

void ErrorMessageToUser(const char* text);

//-----------------------------------------------------------------------------
/// Safe release for COM objects.
template <typename T>
inline void SafeReleaseCom(T*& p) {
    if (p) {
        p->Release();
        p = nullptr;
    }
}

//-----------------------------------------------------------------------------
inline float ClampAngleInDegree(float a) {
    while (a < 0.0f) a += 360.0f;
    while (a > 360.0f) a -= 360.0f;
    return a;
}

//-----------------------------------------------------------------------------
inline float Clamp(float a, float low, float high) {
    if (a < low) a = low;
    else if (a > high) a = high;
    return a;
}

} // end of sare
#endif // SARE_SHARED_FUNCTIONS_H_

