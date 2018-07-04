#ifndef MA_RANGE_RANGE_ADAPTOR_H
#define MA_RANGE_RANGE_ADAPTOR_H

// #include <vector>
// #include <list>

#include <ma/config.h>

#include <ma/range/LinearRange.h>
#include <ma/range/rangeFunction.h>

namespace ma
{
    namespace range
    {
        template<typename> class RangeAdaptor;

        template<>
        class RangeAdaptor<LinearRange> : public LinearRange
        {
        public:
            using LinearRange::LinearRange;

            template<typename LR>
            constexpr explicit RangeAdaptor(LR && lr) noexcept :
                LinearRange(std::forward<LR>(lr))
            {}
            
            using LinearRange::operator[];
        };

        template<typename T>
        class RangeAdaptor
        {
            using const_iterator = typename T::const_iterator;
            using iterator = typename T::iterator;

            T range_;

            bool active_;
            bool contiguous_;
            bool hasStep_;
            DiffT step_;
            SizeT rangedElementNb_;

        public:
            template<typename Range>
            CONSTASSERT explicit RangeAdaptor(Range && range) noexcept :
                range_(std::forward<Range>(range)),
                active_(rangeActive(range_)),
                contiguous_(rangeContiguousFromZero(range_)), hasStep_(rangeHasStep(range_)),
                step_(rangeStep(range_)), rangedElementNb_(rangeRangedElementNb(range_))
            {
                //Check if range is empty because it put the range into the UB world
                assert(!empty(range));
            }

            constexpr const_iterator begin() const
            {
                return begin(range_);
            }

            constexpr const_iterator end() const
            {
                return end(range_);
            }

            constexpr SizeT size() const noexcept
            {
                return size(range_);
            }

            constexpr bool active() const
            {
                return active_;
            }

            constexpr bool complete(SizeT totalLength) const
            {
                return contiguous_ && (T::size() == totalLength);
            }

            constexpr bool hasStep() const
            {
                return hasStep_;
            }

            constexpr SizeT start() const
            {
                return T::front();
            }

            constexpr SizeT stop() const
            {
                return T::back() + step_;
            }

            constexpr DiffT step() const
            {
                return step_;
            }

            constexpr SizeT rangedElementNb() const
            {
                return rangedElementNb_;
            }
        };
    }
}

#endif //MA_RANGE_RANGE_ADAPTOR_H
