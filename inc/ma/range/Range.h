#ifndef MA_RANGE_RANGE_H
#define MA_RANGE_RANGE_H

#include <memory>

#include <ma/iterator/Iterator.h>

#include <ma/range/RangeAdaptor.h>

namespace ma
{
    namespace range
    {
        class RangeInterface;
        class Range;

        using uRange = std::unique_ptr<RangeInterface>;

        class RangeInterface
        {
        public:
            using iterator = iterator::Iterator;

            virtual ~RangeInterface() = default;

            virtual iterator begin() const = 0;
            virtual iterator end() const = 0;
            virtual SizeT at(SizeT pos) const = 0;

            virtual SizeT size() const= 0;

            virtual uRange clone() const = 0;

            virtual bool active() const = 0;
            virtual bool complete(SizeT totalLength) const = 0;
            virtual SizeT rangedElementNb() const = 0;
            virtual uRange selectRange(const RangeInterface & r) const = 0;

            virtual bool hasStep() const = 0;
            virtual SizeT start() const = 0;
            virtual SizeT stop() const = 0;
            virtual DiffT step() const = 0;
        };

        template<typename T>
        class RangeImpl : public RangeInterface
        {
            using range_adaptor = RangeAdaptor<T>;

            range_adaptor range_;

        public:
            template<typename Range>
            constexpr explicit RangeImpl(Range && range) noexcept :
                range_(std::forward<Range>(range))
            {}

            iterator begin() const override
            {
                return iterator(range_.begin());
            }

            iterator end() const override
            {
                return iterator(range_.end());
            }

            SizeT at(SizeT pos) const override
            {
                return range_[pos];
            }

            SizeT size() const override
            {
                return range_.size();
            }

            uRange clone() const override
            {
                return uRange(new RangeImpl(range_));
            }

            bool active() const override
            {
                return range_.active();
            }

            bool complete(SizeT totalLength) const override
            {
                return range_.complete(totalLength);
            }

            SizeT rangedElementNb() const override
            {
                return range_.rangedElementNb();
            }

            uRange selectRange(const RangeInterface & rb) const override
            {
                if(hasStep() && rb.hasStep())
                    return selectLinearRange(rb);
                else
                    return selectVectRange(rb);
            }

            bool hasStep() const override
            {
                return range_.hasStep();
            }

            SizeT start() const override
            {
                return range_.start();
            }

            SizeT stop() const override
            {
                return range_.stop();
            }

            DiffT step() const override
            {
                return range_.step();
            }

            uRange selectLinearRange(const RangeInterface & r) const
            {
                return uRange(new RangeImpl<LinearRange>(
                    LinearRange(range_.start(), range_.stop(), range_.step())
                    .select(r.start(), r.stop(), r.step())
                ));

            }

            uRange selectVectRange(const RangeInterface & r) const
            {
                VectRange res(r.size());

                std::transform(r.begin(), r.end(), res.begin(),
                    [this] (SizeT pos)
                    {
                        assert(pos < range_.size());
                        return range_[pos];
                    }
                );

                return uRange(new RangeImpl<VectRange>(std::move(res)));
            }
        };

        template<typename Range>
        uRange makeRangeImpl(Range && range)
        {
            return uRange(new RangeImpl<decay_t<Range>>(std::forward<Range>(range)));
        }

        template<typename> struct TOTO;

        class Range
        {
        public:
            using const_iterator = iterator::Iterator;
            using iterator = iterator::Iterator;

        protected:
            uRange range_;

            explicit Range(uRange && range) noexcept :
                range_(std::move(range))
            {}

        public:

            explicit Range():
                range_(makeRangeImpl(LinearRange()))
            {}
            explicit Range(SizeT stop):
                range_(makeRangeImpl(LinearRange(stop)))
            {}

            explicit Range(SizeT start, SizeT stop):
                range_(makeRangeImpl(LinearRange(start, stop)))
            {}

            explicit Range(SizeT start, SizeT stop, DiffT step):
                range_(makeRangeImpl(LinearRange(start, stop, step)))
            {}

            template<typename T, typename = IsNotEquivalent<T, Range>, typename = IsIterable<T>>
            explicit Range(T && range):
                range_(makeRangeImpl(range))
            {}

            Range(const Range & range):
                range_(range.clone())
            {}

            Range(Range && range) = default;

            Range & operator=(const Range & range)
            {
                range_ = range.clone();
                return *this;
            }

            Range & operator=(Range && range) = default;

            ~Range() = default;

            iterator begin() const
            {
                return range_->begin();
            }

            iterator end() const
            {
                return range_->end();
            }

            SizeT start() const
            {
                return range_->start();
            }

            SizeT stop() const
            {
                return range_->stop();
            }

            SizeT operator[](SizeT pos) const
            {
                return range_->at(pos);
            }

            SizeT size() const
            {
                return range_->size();
            }

            uRange clone() const
            {
                return range_->clone();
            }

            bool active() const
            {
                return range_->active();
            }

            bool complete(SizeT totalLength) const
            {
                return range_->complete(totalLength);
            }

            SizeT rangedElementNb() const
            {
                return range_->rangedElementNb();
            }

            Range select(const Range & r) const
            {
                return Range(range_->selectRange(*r.range_));
            }

            Range closeAt(SizeT pos) const
            {
                SizeT nStart = range_->start() + pos;
                return Range(nStart, nStart, 1);
            }
        };
    }
}

#endif //MA_RANGE_RANGE_H
