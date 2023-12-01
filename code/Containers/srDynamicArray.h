#ifndef SARE_DYNAMICARRAY_H
#define SARE_DYNAMICARRAY_H

#include "Allocators/SrDefaultAllocator.h"
#include <stdint.h>
#include <vector>
#include <algorithm>


template <typename T,
typename TAllocator = SrDefaultAllocator<T>>
class SrDynamicArray : public std::vector<T, TAllocator>
{

public:
    typedef std::vector<T, TAllocator> BaseClass;
    typedef typename BaseClass::size_type size_type;
    using BaseClass::value_type;
    using BaseClass::pointer;
    using BaseClass::const_pointer;
    using BaseClass::reference;
    using BaseClass::const_reference;
    using BaseClass::iterator;
    using BaseClass::const_iterator;

    using BaseClass::size;
    using BaseClass::capacity;
    using BaseClass::empty;
    using BaseClass::clear;
    using BaseClass::begin;
    using BaseClass::at;
    using BaseClass::front;
    using BaseClass::back;

    bool push_back(const T& value)
    {
        if (size() + 1 > capacity())
        {
            if (!reserve(size() + 1))
            {
                return false;
            }
        }
        BaseClass::push_back(value);
        return true;
    }

    bool reserve(size_type n)
    {
        if (n <= capacity())
            return true;

        size_type nNewCap = capacity();
        if (nNewCap < 16) nNewCap = 16;
        while (nNewCap < n)
        {
            nNewCap += (nNewCap >> 1);
        }
        n = nNewCap;

        return reserve_internal(n);
    }
private:
    bool reserve_internal(size_type n)
    {
        try
        {
            BaseClass::reserve(n);
        }
        catch(const std::bad_alloc& excp)
        {
            return false;
        }
        return true;
    }
};

#endif // SARE_DYNAMICARRAY_H

