#ifndef SARE_DEFAULT_ALLOCATOR_H
#define SARE_DEFAULT_ALLOCATOR_H

#include "MemoryMng/SrMemoryManager.h"
#include <new>
#include <limits>
#include <cstdint>

SrMemoryManagerSPtr SrDefaultAllocator_GetMemMng();

template <class T>
class SrDefaultAllocator
{
 public:
   // type definitions
   typedef T        value_type;
   typedef T*       pointer;
   typedef const T* const_pointer;
   typedef T&       reference;
   typedef const T& const_reference;
   typedef std::uint32_t    size_type;
   typedef std::ptrdiff_t difference_type;

   // rebind allocator to type U
   template <class U>
   struct rebind
   {
       typedef SrDefaultAllocator<U> other;
   };

   // return address of values
   pointer address (reference value) const
   {
       return &value;
   }

   const_pointer address (const_reference value) const
   {
       return &value;
   }

   /* constructors and destructor
    * - nothing to do because the allocator has no state
    */
   SrDefaultAllocator() throw()
   {
   }
   SrDefaultAllocator(const SrDefaultAllocator&) throw()
   {
   }
   template <class U>
    SrDefaultAllocator (const SrDefaultAllocator<U>&) throw()
     {
   }

   ~SrDefaultAllocator() throw()
   {
   }

   // return maximum number of elements that can be allocated
   size_type max_size () const throw() {
       return std::numeric_limits<std::size_t>::max() / sizeof(T);
   }


   pointer allocate (size_type n, const void* = 0)
   {
       SrMemoryManagerSPtr pMm = SrDefaultAllocator_GetMemMng();
       size_type nReqBytes = n * sizeof(T);
       pointer ret = (pointer)pMm->Allocate(nReqBytes);
       return ret;
   }

   // initialize elements of allocated storage p with value value
   void construct (pointer p, const T& value) {
       // initialize memory with placement new
       new((void*)p)T(value);
   }

   // destroy elements of initialized storage p
   void destroy (pointer p) {
       // destroy objects by calling their destructor
       p->~T();
   }

   // deallocate storage p of deleted elements
   void deallocate (pointer p, size_type num)
   {
        SrMemoryManagerSPtr pMm = SrDefaultAllocator_GetMemMng();
        pMm->Free(p);
   }

    bool resizeTmp(size_type n)
    {
        if (0 == n)
        {
            if (m_pTmp)
            {
                SrMemoryManagerSPtr pMm = SrDefaultAllocator_GetMemMng();
                void* p = pMm->Free(m_pTmp);
                m_pTmp = nullptr;
                m_nTmpCapacity = 0;
                return true;
            }
            return true;
        }

        if (n <= m_nTmpCapacity)
            return true;

        SrMemoryManagerSPtr pMm = SrDefaultAllocator_GetMemMng();
        if (m_pTmp)
        {
            void* p = pMm->Reallocate(m_pTmp, n);
            if (!p)
            {
               return false;
            }
            m_pTmp = p;
            m_nTmpCapacity = n;
            return true;
        }

        void* p = pMm->Allocate(n);
        if (!p)
        {
           return false;
        }
        m_pTmp = p;
        m_nTmpCapacity = n;
        return true;
    }
private:
   void* m_pTmp{};
   size_type m_nTmpCapacity{};
};

// return that all specializations of this allocator are interchangeable
template <class T1, class T2>
bool operator== (const SrDefaultAllocator<T1>&,
                const SrDefaultAllocator<T2>&) throw() {
   return true;
}
template <class T1, class T2>
bool operator!= (const SrDefaultAllocator<T1>&,
                const SrDefaultAllocator<T2>&) throw() {
   return false;
}

#endif // SARE_DEFAULT_ALLOCATOR_H

