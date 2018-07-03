#ifndef MA_ITERATOR_LINEAR_ITERATOR_H
#define MA_ITERATOR_LINEAR_ITERATOR_H

#include <ma/type.h>

namespace ma
{
    namespace iterator
    {
        class LinearIterator
        {
            using value_type = SizeT;
            using difference_type = DiffT;
            using pointer = void;
            using reference = void;
            using iterator_category = std::random_access_iterator_tag;

            SizeT start_;
            DiffT step_;

        public:
            constexpr explicit LinearIterator(SizeT start = 0, DiffT step = 1) noexcept:
                start_(start), step_(step)
            {}

            constexpr LinearIterator(const LinearIterator &) noexcept = default;
            LinearIterator & operator=(const LinearIterator &) noexcept = default;

            constexpr DiffT shift(DiffT pos) const
            {
                return step_ * pos;
            }

            constexpr SizeT value(DiffT pos = 0) const
            {
                return start_ + shift(pos);
            }

            constexpr LinearIterator add(DiffT pos) const
            {
                return LinearIterator(value(pos), step_);
            }

            LinearIterator & increase(DiffT pos)
            {
                start_ = value(pos);
                return *this;
            }

            constexpr bool superiorStrict(LinearIterator const & it) const
            {
                return start_ > it.start_;
            }

            constexpr bool equal(LinearIterator const & it) const
            {
                return start_ == it.start_;
            }

            constexpr LinearIterator operator[](DiffT pos) const
            {
                return LinearIterator(value(pos), step_);
            }

            constexpr SizeT operator*() const
            {
                return start_;
            }

            LinearIterator& operator++()
            {
                return increase(1);
            }

            LinearIterator operator++(int)
            {
                LinearIterator oli(start_, step_);
                increase(1);
                return oli;
            }

            LinearIterator& operator--()
            {
                return increase(-1);
            }

            LinearIterator operator--(int)
            {
                LinearIterator oli(start_, step_);
                increase(-1);
                return oli;
            }

            LinearIterator& operator+=(DiffT pos)
            {
                return increase(pos);
            }

            LinearIterator& operator-=(DiffT pos)
            {
                return increase(-pos);
            }

            constexpr LinearIterator operator+(DiffT pos)
            {
                return add(pos);
            }

            constexpr LinearIterator operator-(DiffT pos)
            {
                return add(-pos);
            }

            constexpr DiffT operator-(const LinearIterator & oli)
            {
                return start_ - *oli;
            }
        };


        inline LinearIterator operator+(DiffT pos, LinearIterator li)
        {
            return li.add(pos);
        }


        inline LinearIterator operator-(DiffT pos, LinearIterator li)
        {
            return li.add(-pos);
        }


        inline bool operator>(LinearIterator const & li1, LinearIterator const & li2)
        {
            return li1.superiorStrict(li2);
        }

        inline bool operator<(LinearIterator const & li1, LinearIterator const & li2)
        {
            return li2.superiorStrict(li1);
        }

        inline bool operator>=(LinearIterator const & li1, LinearIterator const & li2)
        {
            return !li2.superiorStrict(li1);
        }

        inline bool operator<=(LinearIterator const & li1, LinearIterator const & li2)
        {
            return !li1.superiorStrict(li2);
        }

        inline bool operator==(LinearIterator const & li1, LinearIterator const & li2)
        {
            return li1.equal(li2);
        }

        inline bool operator!=(LinearIterator const & li1, LinearIterator const & li2)
        {
            return !li1.equal(li2);
        }

        template<typename T>
        using IsNotLinearIterator = enable_if_t<!is_same<T, LinearIterator>::value>;

    }
}

#endif //MA_ITERATOR_LINEAR_ITERATOR_H
