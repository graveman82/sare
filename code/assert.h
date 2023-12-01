#ifndef SARE_ASSERT_H_
#define SARE_ASSERT_H_

#ifdef SARE_DEBUG_LEVEL
#define SARE_DEBUG
#endif

#ifndef SARE_DEBUG
#define BOOST_DISABLE_ASSERTS
#endif
#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>

#define SARE_ASSERT(exp) BOOST_ASSERT(exp)
#define SARE_ASSERT_MSG(exp,msg) BOOST_ASSERT_MSG(exp,msg)

#endif // SARE_ASSERT_H_
