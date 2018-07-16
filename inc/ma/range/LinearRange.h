#ifndef MA_RANGE_LINEAR_RANGE_H
#define MA_RANGE_LINEAR_RANGE_H

#include <ma/type.h>
#include <ma/function.h>

#include <ma/iterator/LinearIterator.h>

namespace ma
{
    namespace range
    {
        constexpr inline SizeT alignStop(SizeT start, SizeT stop, DiffT step)
        {
            return start + step * abs((stop - start) / step);
        }

        class LinearRange
        {
        public:
            using const_iterator = iterator::LinearIterator;
            using iterator = iterator::LinearIterator;

        protected:
            SizeT start_;
            SizeT stop_;
            DiffT step_;

        public:

            constexpr explicit LinearRange() noexcept :
                start_(0), stop_(0), step_(1)
            {}

            constexpr explicit LinearRange(SizeT stop) noexcept :
                start_(0), stop_(stop), step_(1)
            {}

            constexpr explicit LinearRange(SizeT start, SizeT stop) noexcept :
                start_(start), stop_(stop), step_((stop >= start)?1:-1)
            {}

            constexpr explicit LinearRange(SizeT start, SizeT stop, DiffT step) noexcept :
                start_(start), stop_(alignStop(start, stop, step)), step_(step)
            {}

            constexpr LinearRange(const LinearRange &) noexcept = default;
            LinearRange& operator=(const LinearRange &) noexcept = default;

            ~LinearRange() = default;

            constexpr SizeT operator[](SizeT pos) const noexcept
            {
                return start_ + pos * step_;
            }

            constexpr SizeT operator*() const noexcept
            {
                return start_;
            }

            constexpr const_iterator begin() const noexcept
            {
                return const_iterator(start_, step_);
            }

            constexpr const_iterator end() const noexcept
            {
                return const_iterator(stop_, step_);
            }

            constexpr SizeT size() const noexcept
            {
                return (stop_ - start_) / step_;
            }

            constexpr bool empty() const noexcept
            {
                return stop_ == start_;
            }

            constexpr bool active() const noexcept
            {
                return stop_ != start_;
            }

            constexpr bool contiguousFromZero() const noexcept
            {
                return (start_ == 0) && (step_ == 1);
            }

            constexpr bool complete(SizeT totalLength) const noexcept
            {
                return contiguousFromZero() && (stop_ == totalLength);
            }

            constexpr SizeT rangedElementNb() const noexcept
            {
                return (step_ == 1)?size() : 1;
            }

            constexpr bool hasStep() const noexcept
            {
                return true;
            }

            constexpr SizeT front() const noexcept
            {
                return start_;
            }

            constexpr SizeT back() const noexcept
            {
                return stop_;
            }

            constexpr SizeT start() const noexcept
            {
                return start_;
            }

            constexpr SizeT stop() const noexcept
            {
                return stop_;
            }

            constexpr DiffT step() const noexcept
            {
                return step_;
            }

            constexpr LinearRange select(SizeT start, SizeT stop, DiffT step) const noexcept
            {
                return LinearRange(
                    start_ + start * step_,
                    start_ + stop  * step_,
                    DiffT(step_ * step)
                );
            }

            constexpr LinearRange select(const LinearRange & lr) const noexcept
            {
                return select(lr.start_, lr.stop_, lr.step_);
            }

            constexpr LinearRange closeAt(SizeT pos) const noexcept
            {
                return LinearRange(start_ + pos, start_ + pos, 1);
            }
        };

        template<typename T>
        using IsNotLinearRange = enable_if_t<!is_same<T, LinearRange>::value>;
    }

}

#endif //MA_RANGE_LINEAR_RANGE_H
