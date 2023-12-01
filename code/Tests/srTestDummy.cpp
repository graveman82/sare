#include <stdio.h>

#include "Tests/srTestDummy.h"

SrTestDummy::~SrTestDummy()
{
    printf("SrTestDummy -- dtor\n");
}

SrTestDummy::SrTestDummy()
{
    printf("SrTestDummy -- default ctor\n");
}

SrTestDummy::SrTestDummy(const SrTestDummy& oth)
{
    printf("SrTestDummy -- copy ctor\n");
}

SrTestDummy::SrTestDummy(SrTestDummy&& oth)
{
    printf("SrTestDummy -- move ctor\n");
}

SrTestDummy& SrTestDummy::operator=(const SrTestDummy& oth)
{
    if (this == &oth)
        return *this;
    printf("SrTestDummy -- assign operator\n");
    return *this;
}

SrTestDummy& SrTestDummy::operator=(SrTestDummy&& oth)
{
    if (this == &oth)
        return *this;
    printf("SrTestDummy -- move assign operator\n");
    return *this;
}
