#ifndef RANGE_CONTAINER_H
#define RANGE_CONTAINER_H

#include <vector>
#include <list>

#include "LinearRange.h"
#include "rangeFunction.h"

namespace ma
{
    namespace range
    {
        template<typename T> class RangeContainer;

        template<>
        class RangeContainer<LinearRange> : public LinearRange
        {
        public:
            template<typename... Args>
            RangeContainer(Args... args):LinearRange(args...){}
            //using LinearRange::LinearRange;
        };

        template<typename T>
        class RangeContainer : public T
        {
            bool active_;
            bool hasSpace_;
            bool hasStep_;
            DiffT step_;
            SizeT rangedElement_;

        public:
            template<typename Range>
            RangeContainer(Range && range):
                T(range), active_(rangeActive(range)),
                hasSpace_(rangeHasSpace(range)), hasStep_(rangeHasStep(range)),
                step_(rangeStep(range)), rangedElement_(rangeNbRangedElement(range))
            {}

            bool isActive() const
            {
                return active_;
            }

            bool isComplete(SizeT totalLength) const
            {
                return !hasSpace_ && (T::size() == totalLength);
            }

            bool hasStep() const
            {
                return hasStep_;
            }

            SizeT start() const
            {
                return T::front();
            }

            SizeT stop() const
            {
                return T::back() + step_;
            }

            DiffT step() const
            {
                return step_;
            }

            SizeT rangedElement() const
            {
                return rangedElement_;
            }
        };
    }
}

#endif //RANGE_CONTAINER_H
