#ifndef SARE_SINGLETON_H_
#define SARE_SINGLETON_H_

#include "assert.h"

namespace sare {

template <typename T>
class Singleton {
public:
    Singleton(T& object) {
        SARE_ASSERT_MSG(!s_pInstance_, "single instance allowed only!");
        s_pInstance_ = &object;
    }

    virtual ~Singleton() {
        SARE_ASSERT_MSG(s_pInstance_, "no instance available");
        s_pInstance_ = nullptr;
    }

    static T& instance() {
        SARE_ASSERT_MSG(s_pInstance_, "no instance available");
        return *s_pInstance_;
    }

private:
    static T* s_pInstance_;

    Singleton(const Singleton<T>&);
    Singleton& operator=(const Singleton&);
};

//-----------------------------------------------------------------------------
template <typename T> T* Singleton<T>::s_pInstance_ = nullptr;
} // end of sare

#endif // SARE_SINGLETON_H_

