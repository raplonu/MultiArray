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
        
        template<typename> class RangeImpl;

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
            virtual uRange selectRange(const LinearRange & r) const = 0;

            virtual bool hasStep() const = 0;
            virtual SizeT start() const = 0;
            virtual SizeT stop() const = 0;
            virtual DiffT step() const = 0;
        };

        template<typename Range>
        uRange makeRangeImpl(Range && range)
        {
            return uRange(new RangeImpl<decay_t<Range>>(std::forward<Range>(range)));
        }

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

            uRange selectRange(const LinearRange & rb) const override
            {
                if(hasStep())
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

            template<typename R>
            uRange selectLinearRange(const R & r) const
            {
                return uRange(new RangeImpl<LinearRange>(
                    LinearRange(range_.start(), range_.stop(), range_.step())
                    .select(r.start(), r.stop(), r.step())
                ));

            }

            template<typename R>
            uRange selectVectRange(const R & r) const
            {
                VectRange res(r.size());

                std::transform(r.begin(), r.end(), res.begin(),
                    [this] (SizeT pos)
                    {
                        assert(pos < range_.size());
                        return range_[pos];
                    }
                );

                return makeRangeImpl(std::move(res));
            }
        };

        class RangeVariant
        {
            using RangeM = detail::Variant<LinearRange, RangeInterface>;
        public:
            using const_iterator = iterator::Iterator;
            using iterator = iterator::Iterator;

        protected:
            RangeM range_;

            explicit RangeVariant(RangeM && range) noexcept :
                range_(std::move(range))
            {}

        public:

            explicit RangeVariant():
                range_(detail::initSmall)
            {}
            explicit RangeVariant(SizeT stop):
                range_(detail::initSmall, stop)
            {}

            explicit RangeVariant(SizeT start, SizeT stop):
                range_(detail::initSmall, start, stop)
            {}

            explicit RangeVariant(SizeT start, SizeT stop, DiffT step):
                range_(detail::initSmall, start, stop, step)
            {}

            explicit RangeVariant(const LinearRange & range):
                range_(detail::initSmall, range)
            {}

            template<
                typename T,
                typename = IsNotEquivalent<T, RangeVariant>,
                typename = IsNotEquivalent<T, LinearRange>,
                typename = IsIterable<T>>
            explicit RangeVariant(T && range):
                range_(makeRangeImpl(std::forward<T>(range)).release())
            {}

            RangeVariant(const RangeVariant & range):
                range_(range.clone())
            {}

            RangeVariant(RangeVariant && range) = default;

            RangeVariant & operator=(const RangeVariant & range)
            {
                range_ = range.clone();
                return *this;
            }

            RangeVariant & operator=(RangeVariant && range) = default;

            ~RangeVariant() = default;

            iterator begin() const
            {
                return range_.isSmall()
                    ? iterator(range_.small().begin())
                    : range_.big().begin();
            }

            iterator end() const
            {
                return range_.isSmall()
                    ? iterator(range_.small().end())
                    : range_.big().end();
            }

            SizeT start() const
            {
                return range_.isSmall()
                    ? range_.small().start()
                    : range_.big().start();
            }

            SizeT stop() const
            {
                return range_.isSmall()
                    ? range_.small().stop()
                    : range_.big().stop();
            }

            SizeT at(SizeT pos) const
            {
                return range_.isSmall()
                    ? range_.small()[pos]
                    : range_.big().at(pos);
            }

            SizeT size() const
            {
                return range_.isSmall()
                    ? range_.small().size()
                    : range_.big().size();
            }

            RangeM clone() const
            {
                return range_.isSmall()
                    ? RangeM(detail::initSmall, range_.small())
                    : RangeM(range_.big().clone().release());
            }

            bool active() const
            {
                return range_.isSmall()
                    ? range_.small().active()
                    : range_.big().active();
            }

            bool complete(SizeT totalLength) const
            {
                return range_.isSmall()
                    ? range_.small().complete(totalLength)
                    : range_.big().complete(totalLength);
            }

            SizeT rangedElementNb() const
            {
                return range_.isSmall()
                    ? range_.small().rangedElementNb()
                    : range_.big().rangedElementNb();
            }

            RangeVariant selectRange(const RangeVariant & r) const
            {
                if(range_.isSmall())
                {
                    if(r.range_.isSmall())
                        return RangeVariant(range_.small().select(r.range_.small()));
                    else
                        return RangeVariant(RangeImpl<LinearRange>(range_.small()).selectRange(r.range_.big()).release());
                }
                else
                {
                    if(r.range_.isSmall())
                        return RangeVariant(RangeM(range_.big().selectRange(
                            RangeImpl<LinearRange>(r.range_.small())
                        ).release()));
                    else
                        return RangeVariant(RangeM(range_.big().selectRange(
                            r.range_.big()
                        ).release()));
                }
            }

            RangeVariant selectRange(const LinearRange & r) const
            {
                if(range_.isSmall())
                    return RangeVariant(range_.small().select(r));
                else
                    return RangeVariant(RangeM(range_.big().selectRange(
                        RangeImpl<LinearRange>(r)
                    ).release()));
            }

            RangeVariant closeAt(SizeT pos) const
            {
                SizeT nStart = start() + pos;
                return RangeVariant(nStart, nStart, 1);
            }
        };

        class RangeLegacy
        {
        public:
            using const_iterator = iterator::Iterator;
            using iterator = iterator::Iterator;

        protected:
            uRange range_;

            explicit RangeLegacy(uRange && range) noexcept :
                range_(std::move(range))
            {}

        public:
            explicit RangeLegacy(SizeT stop):
                range_(makeRangeImpl(LinearRange(stop)))
            {}

            explicit RangeLegacy(SizeT start, SizeT stop):
                range_(makeRangeImpl(LinearRange(start, stop)))
            {}

            explicit RangeLegacy(SizeT start, SizeT stop, DiffT step):
                range_(makeRangeImpl(LinearRange(start, stop, step)))
            {}

            template<
                typename T, typename BaseT = decay_t<T>,
                typename = IsNotEquivalent<RangeLegacy, BaseT>
            >
            explicit RangeLegacy(T && range):
                range_(makeRangeImpl(std::forward<T>(range)))
            {}

            RangeLegacy(const RangeLegacy & range):
                range_(range.clone())
            {}

            RangeLegacy(RangeLegacy && range) = default;

            RangeLegacy & operator=(const RangeLegacy & range)
            {
                range_ = range.clone();
                return *this;
            }

            RangeLegacy & operator=(RangeLegacy && range) = default;

            ~RangeLegacy() = default;

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

            SizeT at(SizeT pos) const
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

            RangeLegacy selectRange(const RangeLegacy & r) const
            {
                return RangeLegacy(range_->selectRange(*r.range_));
            }

            RangeLegacy selectRange(const LinearRange & r) const
            {
                return RangeLegacy(range_->selectRange(r));
            }

            RangeLegacy closeAt(SizeT pos) const
            {
                SizeT nStart = range_->start() + pos;
                return RangeLegacy(nStart, nStart, 1);
            }
        };


        template<typename Impl>
        class RangeT
        {
        public:
            using const_iterator = iterator::Iterator;
            using iterator = iterator::Iterator;

        protected:
            Impl range_;

            RangeT(Impl && range):
                range_(std::move(range))
            {}

        public:
            explicit RangeT(SizeT stop = 0):
                range_(stop)
            {}

            explicit RangeT(SizeT start, SizeT stop):
                range_(start, stop)
            {}

            explicit RangeT(SizeT start, SizeT stop, DiffT step):
                range_(start, stop, step)
            {}

            template<typename T, typename = IsNotEquivalent<T, RangeT>, typename = IsIterable<T>>
            explicit RangeT(T && range):
                range_(std::forward<T>(range))
            {}

            RangeT(const RangeT & range):
                range_(range.clone())
            {}

            RangeT(RangeT && range) = default;

            RangeT & operator=(const RangeT & range)
            {
                range_ = range.clone();
                return *this;
            }

            RangeT & operator=(RangeT && range) = default;

            ~RangeT() = default;

            iterator begin() const
            {
                return range_.begin();
            }

            iterator end() const
            {
                return range_.end();
            }

            SizeT start() const
            {
                return range_.start();
            }

            SizeT stop() const
            {
                return range_.stop();
            }

            SizeT operator[](SizeT pos) const
            {
                return range_.at(pos);
            }

            SizeT size() const
            {
                return range_.size();
            }

            uRange clone() const
            {
                return range_.clone();
            }

            bool active() const
            {
                return range_.active();
            }

            bool complete(SizeT totalLength) const
            {
                return range_.complete(totalLength);
            }

            SizeT rangedElementNb() const
            {
                return range_.rangedElementNb();
            }

            RangeT select(const RangeT & r) const
            {
                return RangeT(range_.selectRange(r.range_));
            }

            RangeT select(const LinearRange & r) const
            {
                return RangeT(range_.selectRange(r));
            }

            RangeT closeAt(SizeT pos) const
            {
                SizeT nStart = range_.start() + pos;
                return RangeT(nStart, nStart, 1);
            }
        };

        using Range = RangeT<RangeVariant>;
    }
}

#endif //MA_RANGE_RANGE_H
