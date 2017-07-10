#ifndef LINEAR_ITERATOR_H
#define LINEAR_ITERATOR_H

#include <ma/detail/base.h>

namespace ma
{
    namespace iterator
    {
        class LinearIterator
        {
            using IteratorT = LinearIterator;

            SizeT start_;
            DiffT step_;

        public:
            LinearIterator(SizeT start = 0, DiffT step = 1)
                :start_(start), step_(step)
            {}

            LinearIterator(IteratorT const & li) = default;
            IteratorT& operator=(IteratorT const & li) = default;

            DiffT shift(DiffT pos) const
            {
                return step_ * pos;
            }

            SizeT value(DiffT pos = 0) const
            {
                return start_ + shift(pos);
            }

            IteratorT add(DiffT pos) const
            {
                return {value(pos), step_};
            }

            IteratorT& increase(DiffT pos)
            {
                start_ += step_ * pos;
                return *this;
            }

            bool superiorStrict(IteratorT const & it) const
            {
                return start_ > it.start_;
            }

            bool equal(IteratorT const & it) const
            {
                return start_ == it.start_;
            }

            IteratorT operator[](DiffT pos) const
            {
                return {value(pos), step_};
            }

            SizeT operator*() const
            {
                return start_;
            }

            // operator SizeT() const
            // {
            //     return start_;
            // }

            IteratorT& operator++()
            {
                return increase(1);
            }

            IteratorT operator++(int)
            {
                IteratorT oli(start_, step_);
                increase(1);
                return oli;
            }

            IteratorT& operator--()
            {
                return increase(-1);
            }

            IteratorT operator--(int)
            {
                IteratorT oli(start_, step_);
                increase(-1);
                return oli;
            }

            IteratorT& operator+=(DiffT pos)
            {
                return increase(pos);
            }

            IteratorT& operator-=(DiffT pos)
            {
                return increase(-pos);
            }

            IteratorT operator+(DiffT pos)
            {
                return add(pos);
            }

            IteratorT operator-(DiffT pos)
            {
                return add(-pos);
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

    }
}

#endif //LINEAR_ITERATOR_H
