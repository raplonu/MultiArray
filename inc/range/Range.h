#ifndef MA_RANGE_RANGE_H
#define MA_RANGE_RANGE_H

#include <iostream>
#include <type_traits>
#include <algorithm>
#include <memory>

#include <iostream>

#include "../common/traits.h"
#include "../iterator/Iterator.h"

#include "rangeFunction.h"
#include "LinearRange.h"
#include "RangeContainer.h"

namespace ma
{
    namespace range
    {
        using vectorS = std::vector<SizeT>;

        class RangeBase
        {
            using IteratorI = iterator::IteratorInterface;
            using RangeBasePtr = std::unique_ptr<RangeBase>;

        public:
            virtual ~RangeBase(){}

            virtual IteratorI begin() const = 0;
            virtual IteratorI end() const = 0;
            virtual SizeT at(SizeT pos) const = 0;


            virtual SizeT size() const= 0;

            virtual RangeBasePtr makeUniqueCopy() const = 0;

            virtual bool isActive() const = 0;
            virtual bool isComplete(SizeT totalLength) const = 0;
            virtual SizeT nbRangedElement() const = 0;
            virtual RangeBasePtr selectRange(RangeBase const & rb) const = 0;

            //virtual bool isLinear() const = 0;
            virtual bool hasStep() const = 0;
            virtual SizeT start() const = 0;
            virtual SizeT stop() const = 0;
            virtual DiffT step() const = 0;
        };

        template<typename T>
        class RangeRef : public RangeBase
        {
            using RangeM = T;
            using RangeCM = RangeContainer<T>;
            using RangeT = RangeRef<T>;
            using RangePtr = std::unique_ptr<RangeBase>;
            using IteratorI = iterator::IteratorInterface;
            using IteratorT = iterator::Iterator<typename RangeM::const_iterator>;

            RangeCM range_;

        public:
            RangeRef(RangeM range):range_(std::move(range)){}

            virtual ~RangeRef(){}

            virtual IteratorI begin() const
            {
                return IteratorI(IteratorT(range_.begin()));
            }

            virtual IteratorI end() const
            {
                return IteratorI(IteratorT(range_.end()));
            }

            virtual SizeT at(SizeT pos) const
            {
                return range_[pos];
            }

            virtual SizeT size() const
            {
                return range_.size();
            }

            virtual RangePtr makeUniqueCopy() const
            {
                return RangePtr(new RangeT(range_));
            }

            virtual bool isActive() const
            {
                return rangeActive(range_);
            }

            virtual bool isComplete(SizeT totalLength) const
            {
                return rangeComplete(range_, totalLength);
            }

            virtual SizeT nbRangedElement() const
            {
                return rangeNbRangedElement(range_);
            }


            virtual RangePtr selectRange(RangeBase const & rb) const
            {
                if(hasStep() && rb.hasStep())
                    return selectLinearRange(rb);
                else
                    return selectVectRange(rb);
            }

            virtual bool hasStep() const
            {
                return range_.hasStep();
            }

            virtual SizeT start() const
            {
                return range_.start();
            }

            virtual SizeT stop() const
            {
                return range_.stop();
            }

            virtual DiffT step() const
            {
                return range_.step();
            }

            RangePtr selectLinearRange(RangeBase const & rb) const
            {
                return RangePtr(new RangeRef<LinearRange>(LinearRange
                (
                    range_.start() + rb.start() * range_.step(),
                    range_.start() + rb.stop()  * range_.step(),
                    DiffT(range_.step() * rb.step())
                )));

            }

            RangePtr selectVectRange(RangeBase const & rb) const
            {
                SizeT rangeSize(rb.size());
                std::vector<SizeT> res(rangeSize);

                std::transform(rb.begin(), rb.end(), res.begin(),
                    [this](SizeT pos){return range_[pos];}
                );

                return RangePtr(new RangeRef<vectorS>(std::move(res)));
            }
        };

        class Range
        {
        public:
            using IteratorI = iterator::IteratorInterface;
            using RangePtr = std::unique_ptr<RangeBase>;

            using iterator = iterator::IteratorInterface;

        private:
            RangePtr range_;

        public:
            Range()
                :range_(nullptr)
            {}

            Range(SizeT stop)
                :range_(new RangeRef<LinearRange>(LinearRange(stop)))
            {}

            Range(SizeT start, SizeT stop)
                :range_(new RangeRef<LinearRange>({start, stop}))
            {}

            Range(SizeT start, SizeT stop, DiffT step)
                :range_(new RangeRef<LinearRange>({start, stop, step}))
            {}

            template<
                typename T,
                typename = std::enable_if_t<is_iterable<T>::value>
            >
            Range(T const &ra)
                :range_(new RangeRef<T>(ra))
            {}

            Range(RangePtr && range):range_(std::move(range)){}

            Range(Range const & ri)
                :range_(ri.makeUniqueCopy())
            {}

            Range(Range && ri)
                :range_(std::move(ri.range_))
            {}

            ~Range(){}

            Range& operator=(Range const & ri)
            {
                range_ = ri.makeUniqueCopy();
                return *this;
            }

            Range& operator=(Range && ri)
            {
                range_ = std::move(ri.range_);
                return *this;
            }

            IteratorI begin() const
            {
                return range_->begin();
            }

            IteratorI end() const
            {
                return range_->end();
            }

            SizeT operator[](SizeT pos) const
            {
                return range_->at(pos);
            }

            SizeT size() const
            {
                return range_->size();
            }

            virtual RangePtr makeUniqueCopy() const
            {
                return range_->makeUniqueCopy();
            }

            bool isActive() const
            {
                return range_->isActive();
            }

            bool isComplete(SizeT totalLength) const
            {
                return range_->isComplete(totalLength);
            }

            SizeT nbRangedElement() const
            {
                return range_->nbRangedElement();
            }

            Range select(Range const & ri) const
            {
                return {std::move(range_->selectRange(*(ri.range_)))};
            }
        };
    }
}

#endif //MA_RANGE_RANGE_H