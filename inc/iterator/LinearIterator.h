#ifndef LINEAR_ITERATOR_H
#define LINEAR_ITERATOR_H

#include "../common/type.h"

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

            IteratorT operator[](SizeT pos) const
            {
                return {value(pos), step_};
            }

            SizeT operator*() const
            {
                return start_;
            }

            operator SizeT() const
            {
                return start_;
            }

            IteratorT& operator++()
            {
                start_ += step_;
                return *this;
            }

            IteratorT operator++(int)
            {
                IteratorT oli(start_, step_);
                start_ += step_;
                return oli;
            }

            IteratorT& operator--()
            {
                start_ -= step_;
                return *this;
            }

            IteratorT& operator+=(SizeT pos)
            {
                start_ += shift(pos);
                return *this;
            }

            IteratorT& operator-=(SizeT pos)
            {
                return *this += -pos;
            }

            SizeT shift(SizeT pos) const
            {
                return step_ * pos;
            }

            SizeT value(SizeT pos = 0) const
            {
                return start_ + shift(pos);
            }

            IteratorT add(SizeT pos) const
            {
                return {value(pos), step_};
            }

            IteratorT& increase(SizeT pos)
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

            IteratorT operator+(SizeT pos)
            {
                return add(pos);
            }

            IteratorT operator-(SizeT pos)
            {
                return add(-pos);
            }
        };


        inline LinearIterator operator+(SizeT pos, LinearIterator li)
        {
            return li.add(pos);
        }


        inline LinearIterator operator-(SizeT pos, LinearIterator li)
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
