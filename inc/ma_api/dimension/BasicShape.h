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
#ifndef MA_DIMENSION_BASIC_SHAPE_H
#define MA_DIMENSION_BASIC_SHAPE_H

#include <stdexcept>

#include <ma_api/type.h>

namespace ma
{
    namespace dimension
    {
        class BasicShape
        {
        protected:
            SizeT start_;
            SizeT stop_;
            SizeT size_;

        public:
            constexpr explicit BasicShape() noexcept :
                start_(0), stop_(0), size_(0)
            {}

            constexpr explicit BasicShape(SizeT size) noexcept :
                start_(0), stop_(size), size_(size)
            {}

            constexpr explicit BasicShape(SizeT start, SizeT size) noexcept :
                start_(start), stop_(start + size), size_(size)
            {}

            constexpr BasicShape(const BasicShape &) noexcept = default;
            BasicShape & operator=(const BasicShape &) noexcept = default;

        protected:
            constexpr explicit BasicShape(SizeT start, SizeT stop, SizeT size) noexcept :
                start_(start), stop_(stop), size_(size)
            {}

        public:
            constexpr SizeT at(SizeT pos) const noexcept
            {
                return start_ + pos;
            }

            constexpr BasicShape closeAt(SizeT pos) const noexcept
            {
                return BasicShape(start_ + pos, start_ + pos, size_);
            }

            constexpr BasicShape subShape(SizeT start, SizeT stop) const noexcept
            {
                return BasicShape(start_ + start, start_ + stop, size_);
            }

            constexpr BasicShape subShape(SizeT start) const noexcept
            {
                return BasicShape(start_ + start, stop_, size_);
            }

            constexpr SizeT size() const noexcept
            {
                return stop_ - start_;
            }

            constexpr SizeT baseSize() const noexcept
            {
                return size_;
            }

            constexpr SizeT step() const noexcept
            {
                return 1;
            }

            constexpr bool active() const noexcept
            {
                return start_ != stop_;
            }

            constexpr bool contiguous() const noexcept
            {
                return true;
            }

            void reset() noexcept
            {
                start_ = 0;
                stop_ = size_;
            }

            constexpr SizeT start() const noexcept
            {
                return start_;
            }

            constexpr SizeT stop() const noexcept
            {
                return stop_;
            }

            VectRange shape() const
            {
                return VectRange(1, size());
            }

            constexpr SizeT ndim() const noexcept
            {
                return 1;
            }

        };
    }
}
#endif //MA_DIMENSION_BASIC_SHAPE_H
