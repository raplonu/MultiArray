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
            using value_type = typename T::value_type;
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
                massert(!empty(range_));
            }

            constexpr const_iterator begin() const
            {
                return ma::begin(range_);
            }

            constexpr const_iterator end() const
            {
                return ma::end(range_);
            }

            constexpr SizeT size() const noexcept
            {
                return ma::size(range_);
            }

            constexpr bool active() const
            {
                return active_;
            }

            constexpr bool complete(SizeT totalLength) const
            {
                return contiguous_ && (size() == totalLength);
            }

            constexpr bool hasStep() const
            {
                return hasStep_;
            }

            constexpr SizeT start() const
            {
                return range_.front();
            }

            constexpr SizeT stop() const
            {
                return range_.back() + step_;
            }

            constexpr DiffT step() const
            {
                return step_;
            }

            constexpr SizeT rangedElementNb() const
            {
                return rangedElementNb_;
            }

            constexpr value_type operator[](SizeT pos) const
            {
                return range_[pos];
            }
        };
    }
}

#endif //MA_RANGE_RANGE_ADAPTOR_H
