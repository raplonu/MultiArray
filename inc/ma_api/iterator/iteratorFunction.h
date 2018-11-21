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
#ifndef MA_ITERATOR_ITERATOR_FUNCTION_H
#define MA_ITERATOR_ITERATOR_FUNCTION_H

#include <ma_api/type.h>
#include <ma_api/iterator/LinearIterator.h>

namespace ma
{
    namespace iterator
    {
        constexpr inline int iteratorId(LinearIterator it) noexcept
        {
            return *it;
        }


        template<typename T, typename = IsNotLinearIterator<T>>
        constexpr std::ptrdiff_t iteratorId(const T & it) noexcept
        {
            //access address as ID to compare 2 iterator
            return reinterpret_cast<std::ptrdiff_t>(&(*it));
        }
    }
}

#endif //MA_ITERATOR_ITERATOR_FUNCTION_H
