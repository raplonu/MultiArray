#ifndef MA_CONFIG_H
#define MA_CONFIG_H

//#define NDEBUG
#include <cassert>

#define MA_CXX14 (__cplusplus >= 201402L)
#define MA_CXX17 (__cplusplus >= 201703L)

/**
 * Multi line constexpr only enable for c++ 14 and upper, else disabled
 **/
#if MA_CXX14
#define MLCONSTEXPR constexpr
#else
#define MLCONSTEXPR
#endif

#endif //MA_CONFIG_H