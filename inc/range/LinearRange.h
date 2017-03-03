#ifndef MA_RANGE_LINEAR_RANGE_H
#define MA_RANGE_LINEAR_RANGE_H

#include "../common/type.h"
#include "../common/function.h"
#include "../iterator/LinearIterator.h"

namespace ma
{
    namespace range
    {
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

            LinearRange():start_(0), stop_(0), step_(1)
            {}

            LinearRange(SizeT stop)
                :start_(0), stop_(stop), step_((stop >= 0)?1:-1)
            {}

            LinearRange(SizeT start, SizeT stop)
                :start_(start), stop_(stop), step_((stop >= start)?1:-1){}

            LinearRange(SizeT start, SizeT stop, DiffT step)
                :start_(start), stop_(stop), step_(step)
            {
                DiffT mod(((DiffT)stop - (DiffT)start) % step);
                if(mod != 0)
                    stop_ +=  step - mod;
            }

            LinearRange(LinearRange const & lr) = default;
            LinearRange& operator=(LinearRange const & li) = default;

            SizeT operator[](SizeT pos) const
            {
                return start_ + pos * step_;
            }

            SizeT operator*() const
            {
                return start_;
            }

            const_iterator begin() const
            {
                return {start_, step_};
            }

            const_iterator end() const
            {
                return {stop_, step_};
            }

            SizeT size() const
            {
                return (DiffT(stop_) - DiffT(start_)) / step_;
            }

            bool isActive() const
            {
                return stop_ != start_;
            }

            bool isComplete(SizeT totalLength) const
            {
                return (start_ == 0)
                && (stop_ == totalLength)
                && (step_ == 1);
            }

            SizeT nbRangedElement() const
            {
                return (step_ == 1)?size() : 1;
            }

            bool hasStep() const
            {
                return true;
            }

            SizeT start() const
            {
                return start_;
            }

            SizeT stop() const
            {
                return stop_;
            }

            DiffT step() const
            {
                return step_;
            }

            LinearRange select(SizeT start, SizeT stop, SizeT step) const
            {
                return {
                    start_ + start * step_,
                    start_ + stop  * step_,
                    DiffT(step_ * step)
                };
            }

            LinearRange select(LinearRange const & lr) const
            {
                return select(lr.start_, lr.stop_, lr.step_);
            }

        };
    }

}

#endif //MA_RANGE_LINEAR_RANGE_H
