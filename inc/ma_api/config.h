#ifndef MA_CONFIG_H
#define MA_CONFIG_H

// #define NDEBUG
#include <cassert>      // assert

/**
 * Replace ((void)0) to nothing to allow constexpr with assert
 **/
#ifdef NDEBUG
#define massert(condition)
#else
#define massert(condition) assert(condition)
#endif

#ifdef NDEBUG
#define CONSTASSERT constexpr
#else
#define CONSTASSERT
#endif


#define MA_CXX17 (__cplusplus >= 201703L)
#define MA_CXX14 (__cplusplus >= 201402L && ! MA_CXX17)

/**
 * Multi line constexpr only enable for c++ 14 and upper, else disabled
 **/
#if MA_CXX14
#define CONSTEXPR14 constexpr
#else
#define CONSTEXPR14
#endif

#if MA_CXX17
#define MAYBE_UNUSED [[maybe_unused]]
#else
#define MAYBE_UNUSED
#endif

#define LIGHTVARIANT

#endif //MA_CONFIG_H