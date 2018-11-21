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
#ifndef MA_ITERATOR_LINEAR_ITERATOR_H
#define MA_ITERATOR_LINEAR_ITERATOR_H

#include <ma_api/type.h>
#include <ma_api/traits.h>

namespace ma
{
    namespace iterator
    {
        /**
         * @brief Iterator defined by a position and a step
         * 
         */
        class LinearIterator
        {
            using value_type = SizeT;
            using difference_type = DiffT;
            using pointer = void;
            using reference = void;
            using iterator_category = std::random_access_iterator_tag;

            SizeT start_;
            HDiffT step_;
            MA_MAYBE_UNUSED HDiffT __nothing; //discard 2 bytes in order tor put the variant test bit

        public:
            constexpr explicit LinearIterator(SizeT start = 0, DiffT step = 1) noexcept :
                start_(start), step_(step), __nothing()
            {}

            constexpr LinearIterator(const LinearIterator &) noexcept = default;
            LinearIterator & operator=(const LinearIterator &) noexcept = default;

            constexpr DiffT shift(DiffT pos) const noexcept
            {
                return step_ * pos;
            }

            constexpr SizeT value(DiffT pos = 0) const noexcept
            {
                return start_ + shift(pos);
            }

            constexpr LinearIterator add(DiffT pos) const noexcept
            {
                return LinearIterator(value(pos), step_);
            }

            LinearIterator & increase(DiffT pos) noexcept
            {
                // (void)nothing_;
                start_ = value(pos);
                return *this;
            }

            constexpr bool superiorStrict(LinearIterator const & it) const noexcept
            {
                return start_ > it.start_;
            }

            constexpr bool equal(LinearIterator const & it) const noexcept
            {
                return start_ == it.start_;
            }

            constexpr LinearIterator operator[](DiffT pos) const noexcept
            {
                return LinearIterator(value(pos), step_);
            }

            constexpr SizeT operator*() const noexcept
            {
                return start_;
            }

            LinearIterator& operator++() noexcept
            {
                return increase(1);
            }

            LinearIterator operator++(int) noexcept
            {
                LinearIterator oli(start_, step_);
                increase(1);
                return oli;
            }

            LinearIterator& operator--() noexcept
            {
                return increase(-1);
            }

            LinearIterator operator--(int) noexcept
            {
                LinearIterator oli(start_, step_);
                increase(-1);
                return oli;
            }

            LinearIterator& operator+=(DiffT pos) noexcept
            {
                return increase(pos);
            }

            LinearIterator& operator-=(DiffT pos) noexcept
            {
                return increase(-pos);
            }

            constexpr LinearIterator operator+(DiffT pos) const noexcept
            {
                return add(pos);
            }

            constexpr LinearIterator operator-(DiffT pos) const noexcept
            {
                return add(-pos);
            }

            constexpr DiffT operator-(const LinearIterator & oli) const noexcept
            {
                return start_ - *oli;
            }
        };

        inline LinearIterator operator+(DiffT pos, const LinearIterator & li) noexcept
        {
            return li.add(pos);
        }


        inline LinearIterator operator-(DiffT pos, const LinearIterator & li) noexcept
        {
            return li.add(-pos);
        }


        inline bool operator>(const LinearIterator & li1, const LinearIterator & li2) noexcept
        {
            return li1.superiorStrict(li2);
        }

        inline bool operator<(LinearIterator const & li1, LinearIterator const & li2) noexcept
        {
            return li2.superiorStrict(li1);
        }

        inline bool operator>=(LinearIterator const & li1, LinearIterator const & li2) noexcept
        {
            return !li2.superiorStrict(li1);
        }

        inline bool operator<=(LinearIterator const & li1, LinearIterator const & li2) noexcept
        {
            return !li1.superiorStrict(li2);
        }

        inline bool operator==(LinearIterator const & li1, LinearIterator const & li2) noexcept
        {
            return li1.equal(li2);
        }

        inline bool operator!=(LinearIterator const & li1, LinearIterator const & li2) noexcept
        {
            return !li1.equal(li2);
        }

        template<typename T>
        using IsNotLinearIterator = IsNotSame<T, LinearIterator>;

    }
}

namespace std
{
    template<> struct iterator_traits<ma::iterator::LinearIterator>
    {
            using value_type = ma::SizeT;
            using difference_type = ma::DiffT;
            using pointer = void;
            using reference = void;
            using iterator_category = std::random_access_iterator_tag;
    };
}

#endif //MA_ITERATOR_LINEAR_ITERATOR_H
