#ifndef MA_ITERATOR_SHAPE_ITERATOR_H
#define MA_ITERATOR_SHAPE_ITERATOR_H

#include <ma_api/type.h>
#include <ma_api/traits.h>

namespace ma
{
    namespace iterator
    {
        template<typename Iterator, typename Shape>
        class ShapeIterator
        {
            using iterator_trait = std::iterator_traits<Iterator>;
        public:

            using value_type        = typename iterator_trait::value_type;
            using pointer           = typename iterator_trait::pointer;
            using reference         = typename iterator_trait::reference;
            using difference_type   = typename iterator_trait::difference_type;
            using iterator_category = typename iterator_trait::iterator_category;

        protected:
            Iterator iterator_;
            const Shape & shape_;
            difference_type pos_;

        public:
            constexpr explicit ShapeIterator(Iterator iterator, const Shape & shape, difference_type pos = 0) noexcept :
                iterator_(iterator), shape_(shape), pos_(pos)
            {}

            constexpr ShapeIterator(const ShapeIterator &) noexcept = default;
            constexpr ShapeIterator(ShapeIterator &&) noexcept = default;

            ShapeIterator& operator=(const ShapeIterator &) noexcept = default;
            ShapeIterator& operator=(ShapeIterator &&) noexcept = default;

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

            ShapeIterator& operator++() noexcept
            {
                ++pos_;
                return *this;
            }

            ShapeIterator operator++(int) noexcept
            {
                return ShapeIterator(iterator_, shape_, pos_++);
            }

            ShapeIterator& operator--() noexcept
            {
                --pos_;
                return *this;
            }

            ShapeIterator operator--(int) noexcept
            {
                return ShapeIterator(iterator_, shape_, pos_--);
            }

            ShapeIterator& operator+=(difference_type pos) noexcept
            {
                pos_ += pos;
                return *this;
            }

            ShapeIterator& operator-=(difference_type pos) noexcept
            {
                pos_ -= pos;
                return *this;
            }

            ShapeIterator operator+(difference_type pos) const noexcept
            {
                return ShapeIterator(iterator_, shape_, pos_ + pos);
            }

            ShapeIterator operator-(difference_type pos) const noexcept
            {
                return ShapeIterator(iterator_, shape_, pos_ - pos);
            }

            bool equal(ShapeIterator const & si) const noexcept
            {
                return pos_at(0) == si.pos_at(0);
            }

            bool operator==(ShapeIterator const & si) const noexcept
            {
                return equal(si);
            }

            bool operator!=(ShapeIterator const & si) const noexcept
            {
                return !equal(si);
            }
        };
    }
}

namespace std
{

    template<typename Iterator, typename Shape>
    struct iterator_traits<ma::iterator::ShapeIterator<Iterator, Shape>>
    {
    using ShapeIterator = ma::iterator::ShapeIterator<Iterator, Shape>;

    using value_type = typename ShapeIterator::value_type;
    using difference_type =	typename ShapeIterator::difference_type;
    using pointer =	typename ShapeIterator::pointer;
    using reference = typename ShapeIterator::reference;
    using iterator_category = typename ShapeIterator::iterator_category;

    };
}

#endif //MA_ITERATOR_SHAPE_ITERATOR_H
