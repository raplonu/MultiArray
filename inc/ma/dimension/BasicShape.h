#ifndef MA_DIMENSION_BASIC_SHAPE_H
#define MA_DIMENSION_BASIC_SHAPE_H

#include <stdexcept>

#include <ma/type.h>

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

            constexpr bool contigous() const noexcept
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

        };
    }
}
#endif //MA_DIMENSION_BASIC_SHAPE_H
