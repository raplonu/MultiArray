#ifndef MA_DIMENSION_DIMENSION_H
#define MA_DIMENSION_DIMENSION_H

#include <ma/type.h>

namespace ma
{
    namespace dimension
    {
        template<typename T>
        class Dimension
        {
        public:

            using iterator       = typename T::iterator;
            using const_iterator = typename T::const_iterator;

        private:
            T range_;
            SizeT length_;

            template<typename TT>
            constexpr explicit Dimension(TT && range, SizeT size) noexcept :
                range_(std::forward<TT>(range)), length_(size)
            {}

        public:
            constexpr explicit Dimension() noexcept :
                range_(), length_(0)
            {}

            constexpr Dimension(SizeT length) noexcept :
                range_(length), length_(length)
            {}

            constexpr Dimension(const Dimension &) noexcept = default;
            constexpr Dimension(Dimension &&) noexcept = default;

            Dimension& operator=(const Dimension &) noexcept = default;
            Dimension& operator=(Dimension &&) noexcept = default;

            Dimension & reset() noexcept
            {
                range_ = T(length_);
                return *this;
            }

            constexpr SizeT size() const noexcept
            {
                return range_.size();
            }

            constexpr SizeT baseSize() const noexcept
            {
                return length_;
            }

            constexpr const_iterator begin() const noexcept
            {
                return range_.begin();
            }

            iterator begin() noexcept
            {
                return range_.begin();
            }

            constexpr const_iterator end() const noexcept
            {
                return range_.end();
            }

            iterator end() noexcept
            {
                return range_.end();
            }

            constexpr SizeT front() const noexcept
            {
                return range_.front();
            }

            constexpr SizeT back() const noexcept
            {
                return range_.back();
            }

            constexpr SizeT operator[](SizeT pos) const noexcept
            {
                return range_[pos];
            }

            constexpr bool complete() const noexcept
            {
                return range_.complete(baseSize());
            }

            constexpr bool active() const noexcept
            {
                return size() != 0;
            }

            constexpr SizeT rangedElementNb() const noexcept
            {
                return range_.rangedElementNb();
            }

            constexpr Dimension select(const T & range) const noexcept
            {
                return (range.active()) ? Dimension(range_.select(range), length_) : closeAt(range_[range.front());

            }

            constexpr Dimension closeAt(SizeT pos) const noexcept
            {
                return Dimension(range_.select(T(pos, pos)), length_);
            }
        };
    }
}

#endif //MA_DIMENSION_DIMENSION_H
