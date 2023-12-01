#ifndef SARE_INTEGERS_H
#define SARE_INTEGERS_H

#ifdef __GNUC__
#   include <stdint.h>
#endif // __GNUC__


namespace sare
{
#ifdef __GNUC__
typedef int32_t Int32;
typedef int16_t Int16;
typedef int8_t Int8;

typedef uint32_t UInt32;
typedef uint16_t UInt16;
typedef uint8_t UInt8;

// index
typedef Int32 Index32;
typedef Int16 Index16;
typedef Int8 Index8;
using Index = Index32;

typedef UInt32 UIndex32;
typedef UInt16 UIndex16;
typedef UInt8 UIndex8;
using UIndex = UIndex32;

// count
typedef Int32 Count32;
typedef Int16 Count16;
typedef Int8 Count8;
using Count = Count32;

typedef UInt32 UCount32;
typedef UInt16 UCount16;
typedef UInt8 UCount8;
using UCount = UCount32;

#endif // __GNUC__
//
} // end of sare

#endif // SARE_INTEGERS_H

