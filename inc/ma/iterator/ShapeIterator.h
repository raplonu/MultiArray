#ifndef MA_ITERATOR_SHAPE_ITERATOR_H
#define MA_ITERATOR_SHAPE_ITERATOR_H

#include <vector>
#include <iterator>
#include <algorithm>

#include <ma/detail/base.h>

namespace ma
{
    namespace iterator
    {
        template<typename RAI, typename Shape>
        class ShapeIterator
        {
            using RAIT = std::iterator_traits<RAI>;
            using ShapeT = Shape;
            using ShapeIteratorT = ShapeIterator<RAI, ShapeT>;

        public:
            using value_type = typename RAIT::value_type;
            using pointer = typename RAIT::pointer;
            using reference = typename RAIT::reference;
            using difference_type = typename RAIT::difference_type;
            using iterator_category = typename RAIT::iterator_category;

        protected:
            RAI iterator_;
            ShapeT const & shape_;
            difference_type pos_;

        public:
            ShapeIterator(RAI iterator, ShapeT const & shape, difference_type pos = 0):
                iterator_(iterator), shape_(shape), pos_(pos)
            {}

            difference_type pos_at(difference_type pos) const
            {
                return shape_.at(pos_ + pos);
            }

            reference reference_at(difference_type pos) const
            {
                return *(iterator_ + pos_at(pos));
            }

            reference operator*() const
            {
                return reference_at(0);
            }

            operator difference_type() const
            {
                return pos_at(0);
            }

            ShapeIteratorT& operator++()
            {
                ++pos_;
                return *this;
            }

            ShapeIteratorT operator++(int)
            {
                return ShapeIteratorT(iterator_, shape_, pos_++);
            }

            ShapeIteratorT& operator--()
            {
                --pos_;
                return *this;
            }

            ShapeIteratorT operator--(int)
            {
                return ShapeIteratorT(iterator_, shape_, pos_--);
            }

            ShapeIteratorT& operator+=(difference_type pos)
            {
                pos_ += pos;
                return *this;
            }

            ShapeIteratorT& operator-=(difference_type pos)
            {
                pos_ -= pos;
                return *this;
            }

            ShapeIteratorT operator+(difference_type pos) const
            {
                return ShapeIteratorT(iterator_, shape_, pos_ + pos);
            }

            ShapeIteratorT operator-(difference_type pos) const
            {
                return ShapeIteratorT(iterator_, shape_, pos_ - pos);
            }

            bool equal(ShapeIteratorT const & si) const
            {
                return pos_at(0) == si.pos_at(0);
            }

            bool operator==(ShapeIteratorT const & si) const
            {
                return equal(si);
            }

            bool operator!=(ShapeIteratorT const & si) const
            {
                return !equal(si);
            }
        };
    }
}

namespace std
{
    template<typename RAI, typename Shape>
    struct iterator_traits<ma::iterator::ShapeIterator<RAI, Shape>>
    {
        using RAIT = std::iterator_traits<RAI>;
        using ShapeT = Shape;
        using ShapeIteratorT = ma::iterator::ShapeIterator<RAI, ShapeT>;

        using difference_type =	typename ShapeIteratorT::difference_type;
        using value_type = typename ShapeIteratorT::value_type;
        using pointer =	typename ShapeIteratorT::pointer;
        using reference = typename ShapeIteratorT::reference;
        using iterator_category = typename ShapeIteratorT::iterator_category;

    };
}

#endif //MA_ITERATOR_SHAPE_ITERATOR_H
