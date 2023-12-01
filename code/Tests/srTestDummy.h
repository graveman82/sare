#ifndef SARE_TESTDUMMY_H
#define SARE_TESTDUMMY_H

#include <stdint.h>

struct SrTestDummy
{
    ~SrTestDummy();

    SrTestDummy();

    SrTestDummy(const SrTestDummy& oth);

    SrTestDummy(SrTestDummy&& oth);

    SrTestDummy& operator=(const SrTestDummy& oth);

    SrTestDummy& operator=(SrTestDummy&& oth);

    int i1;
    char c1;
    char* p;

};

#endif // SARE_TESTDUMMY_H

