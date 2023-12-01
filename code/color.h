#ifndef SARE_COLOR_H
#define SARE_COLOR_H


#include <stdint.h>  // uint8_t

namespace sare {

template <typename T>
struct RgbColor {
    RgbColor(T r, T g, T b) : r(r), g(g), b(b) {}

    T r;
    T g;
    T b;
};

template <typename T>
struct RgbaColor {
    RgbaColor(T r, T g, T b, T a) : r(r), g(g), b(b), a(a) {}

    T r;
    T g;
    T b;
    T a;
};

template <typename T>
struct ArgbColor {
    T a;
    T r;
    T g;
    T b;
};

template <typename T>
struct BgrColor {
    T b;
    T g;
    T r;
};

template <typename T>
struct BgraColor {
    T b;
    T g;
    T r;
    T a;
};

template <typename T>
struct AbgrColor {
    T a;
    T b;
    T g;
    T r;
};

typedef RgbColor<uint8_t> RgbColori;
typedef RgbaColor<uint8_t> RgbaColori;
typedef ArgbColor<uint8_t> ArgbColori;

typedef BgrColor<uint8_t> BgrColori;
typedef BgraColor<uint8_t> BgraColori;
typedef AbgrColor<uint8_t> AbgrColori;

typedef RgbColor<float> RgbColorf;
typedef RgbaColor<float> RgbaColorf;
typedef ArgbColor<float> ArgbColorf;

typedef BgrColor<float> BgrColorf;
typedef BgraColor<float> BgraColorf;
typedef AbgrColor<float> AbgrColorf;

const float kColorByteToFloat = 1.0 / 255.0;
const float kColorFloatToByte = 255.0;

} // end of sare
#endif // SARE_COLOR_H
