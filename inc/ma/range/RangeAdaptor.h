#ifndef MA_RANGE_RANGE_ADAPTOR_H
#define MA_RANGE_RANGE_ADAPTOR_H

// #include <vector>
// #include <list>

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
        };

        template<typename T>
        class RangeAdaptor
        {
            T range_;

            bool active_;
            bool contiguous_;
            bool hasStep_;
            DiffT step_;
            SizeT rangedElement_;

        public:
            template<typename Range>
            constexpr explicit RangeAdaptor(Range && range) noexcept :
                range_(std::forward<Range>(range)),
                active_(rangeActive(range_)),
                contiguous_(rangeContiguous(range_)), hasStep_(rangeHasStep(range_)),
                step_(rangeStep(range_)), rangedElement_(rangeNbRangedElement(range_))
            {}

            constexpr bool isActive() const
            {
                return active_;
            }

            constexpr bool isComplete(SizeT totalLength) const
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

            constexpr SizeT rangedElement() const
            {
                return rangedElement_;
            }
        };
    }
}

#endif //MA_RANGE_RANGE_ADAPTOR_H
