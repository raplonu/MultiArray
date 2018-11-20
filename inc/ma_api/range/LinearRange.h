#ifndef MA_RANGE_LINEAR_RANGE_H
#define MA_RANGE_LINEAR_RANGE_H

#include <ma_api/type.h>
#include <ma_api/function.h>

#include <ma_api/iterator/LinearIterator.h>

namespace ma
{
    namespace range
    {
        /**
         * @brief Find new stop = start + step * K where K is integer and stop <= new stop < stop + step
         * 
         * @param start 
         * @param stop 
         * @param step 
         * @return SizeT  new stop
         */
        constexpr inline SizeT alignStop(SizeT start, SizeT stop, DiffT step)
        {
            return start + step * ma::abs((stop - start) / step);
        }

        /**
         * @brief Represent a range with a start position, a step and a stop position
         * Note: the stop position is not necessary the same as the one specified in constructor
         * 
         */
        class LinearRange
        {
        public:
            using const_iterator = iterator::LinearIterator;
            using iterator = iterator::LinearIterator;

        protected:
            SizeT start_;
            SizeT stop_;
            HDiffT step_;
            /**
             * @brief 
             * 
             */
            // MA_MAYBE_UNUSED HDiffT __nothing = 0;

        public:
            /**
             * @brief Construct a LinearRange with start = stop = 0
             * 
             */
            constexpr explicit LinearRange() noexcept :
                start_(0), stop_(0), step_(1)
            {}

            /**
             * @brief Construct a new Linear Range object of stop iteration that start at 0
             * 
             * @param stop the number of iteration
             */
            constexpr explicit LinearRange(SizeT stop) noexcept :
                start_(0), stop_(stop), step_(1)
            {}


            /**
             * @brief Construct a new Linear Range object that start at start and do stop - start iterations
             * 
             * @param start start position
             * @param stop stop position
             */
            constexpr explicit LinearRange(SizeT start, SizeT stop) noexcept :
                start_(start), stop_(stop), step_((stop >= start)?1:-1)
            {}


            /**
             * @brief Construct a new Linear Range object that start at start and do (stop - start) / step iterations
             * 
             * @param start start postion
             * @param stop stop position
             * @param step step size
             */
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

            /**
             * @brief returns an iterator to the beginning 
             * 
             * @return const_iterator 
             */
            constexpr const_iterator begin() const noexcept
            {
                return const_iterator(start_, step_);
            }

            /**
             * @brief returns an iterator to the end
             * 
             * @return const_iterator 
             */
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
        using IsNotLinearRange = IsNotSame<T, LinearRange>;
    }

}

#endif //MA_RANGE_LINEAR_RANGE_H
