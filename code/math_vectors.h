#ifndef SARE_MATH_VECTORS_H_
#define SARE_MATH_VECTORS_H_


#include <d3dx9math.h>

namespace sare {

//-----------------------------------------------------------------------------

struct D3dxVector2Ex : public D3DXVECTOR2 {
    D3dxVector2Ex() { x = y = 0.0f; }
    D3dxVector2Ex(float x, float y) { this->x = x; this->y = y; }
    void Set(float x, float y) { this->x = x; this->y = y; }
    D3DXVECTOR2& operator=(const D3DXVECTOR2& oth) {
        return reinterpret_cast<D3DXVECTOR2&>(*this) = oth;
    }
};

//-----------------------------------------------------------------------------

struct D3dxVector3Ex : public D3DXVECTOR3 {
    D3dxVector3Ex() { x = y = z = 0.0f; }
    D3dxVector3Ex(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
    D3dxVector3Ex(const D3DXVECTOR3& oth) {
        (*this) = oth;
    }

    void Set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
    D3DXVECTOR3& operator=(const D3DXVECTOR3& oth) {
        return reinterpret_cast<D3DXVECTOR3&>(*this) = oth;
    }
};

//-----------------------------------------------------------------------------

struct D3dxVector4Ex : public D3DXVECTOR4 {
    D3dxVector4Ex() { x = y = z = w = 0.0f; }
    D3dxVector4Ex(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w; }
    void Set(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w; }
    D3DXVECTOR4& operator=(const D3DXVECTOR4& oth) {
        return reinterpret_cast<D3DXVECTOR4&>(*this) = oth;
    }
};

//-----------------------------------------------------------------------------
//
inline float Dot( const D3dxVector3Ex& v1, const D3dxVector3Ex& v2 )
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
} // end of sare


#endif // SARE_MATH_VECTORS_H_
