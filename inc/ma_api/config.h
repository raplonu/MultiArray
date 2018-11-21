/**********************************************************************************
 * Copyright (c) 2016-2019 Julien Bernard, https://github.com/raplonu/MultiArray
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************/
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

#if __cplusplus < 201402L
    #error MutiArray need to be at least C++ 14
#endif

#define MA_CXX17 (__cplusplus >= 201703L)
#define MA_CXX14 (__cplusplus >= 201402L && ! MA_CXX17)

/**
 * Multi line constexpr only enable for c++ 14 and upper
 **/
#define MA_CONSTEXPR constexpr

#if MA_CXX17
#define MA_MAYBE_UNUSED [[maybe_unused]]
#else
#define MA_MAYBE_UNUSED
#endif

#define LIGHTVARIANT

#endif //MA_CONFIG_H