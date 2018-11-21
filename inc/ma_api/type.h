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
#ifndef MA_TYPE_H
#define MA_TYPE_H

#include <cstddef>      // size_t & ptrdiff_t
#include <cstdint>      // size_t & ptrdiff_t
#include <memory>       // allocator

#include <vector>       // vector
#include <array>        // array

namespace ma
{
    using SizeT = std::ptrdiff_t;
    using DiffT = std::ptrdiff_t;

    // TODO define HDiffT to be half size of DiffT
    using HDiffT = std::int16_t;

    enum All{all, aut};
    enum Delay{delay, pass};

    template<typename T>
    using DefaultAlloc = std::allocator<T>;

    using VectRange = std::vector<int>;

    template<int N>
    using ArrayRange = std::array<int, N>;
}

#endif //MA_TYPE_H
