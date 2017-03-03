#ifndef MA_DIMENSION_DIMENSION_H
#define MA_DIMENSION_DIMENSION_H

#include <iostream>

#include "../range/rangeFunction.h"

namespace ma
{
    namespace dimension
    {
        template<typename Dim, typename L>
        void resetDimVect(std::vector<Dim> & dims, L const & lengths)
        {
            std::transform(
                lengths, lengths + dims.size(), dims.begin(),
                [](SizeT length){return Dim(length);}
            );
        }

        template<typename T, typename L>
        std::vector<T> initDimVect(std::vector<L> const & lengths)
        {
            std::vector<T> res(lengths.size());

            resetDimVect<T>(res, lengths.cbegin());

            return res;
        }

        template<typename T, typename L, SizeT NbDim>
        std::vector<T> initDimVect(L const (&lengths)[NbDim])
        {
            std::vector<T> res(NbDim);

            resetDimVect<T>(res, lengths);

            return res;
        }

        template<typename T>
        class Dimension
        {
        public:
            using RangeT = T;
            using DimensionT = Dimension<T>;
            using iterator = typename RangeT::iterator;

        private:
            RangeT range_;
            SizeT length_;

        public:
            Dimension():range_(), length_(0){}

            Dimension(SizeT length):
                range_(length), length_(length)
            {}

            Dimension(RangeT const & range):
                range_(range), length_(range.size())
            {}

            Dimension(SizeT length, RangeT const & range):
                range_(range), length_(length)
            {}

            Dimension(SizeT length, RangeT && range):
                range_(std::move(range)), length_(length)
            {}

            template<typename... Args>
            Dimension(SizeT length, Args... args):
                range_(args...), length_(length)
            {}

            Dimension(DimensionT const & oDim) = default;
            Dimension(DimensionT && oDim) = default;

            DimensionT& operator=(DimensionT const & oDim) = default;
            DimensionT& operator=(DimensionT & oDim) = default;

            DimensionT& reset()
            {
                range_ = RangeT(length_);
                return *this;
            }

            SizeT size() const
            {
                return range_.size();
            }

            SizeT realSize() const
            {
                return length_;
            }

            iterator begin() const
            {
                return range_.begin();
            }

            iterator end() const
            {
                if(range_.begin() != range_.end())
                    return range_.end();
                else
                    return range_.end() + 1ul;
            }

            SizeT operator[](SizeT pos) const
            {
                return range_[pos];
            }

            bool isComplete() const
            {
                return range_.isComplete(realSize());
            }

            bool isActive() const
            {
                return size() != 0;
            }

            SizeT nbRangedElement() const
            {
                return range_.nbRangedElement();
            }

            DimensionT select(DimensionT const & dim) const
            {
                if(!dim.isActive())
                    return closeAt(range_[dim[0]]);
                else

                    return DimensionT(length_, range_.select(dim.range_));
            }

            DimensionT closeAt(SizeT pos) const
            {
                return DimensionT(length_, range_.select(RangeT(pos, pos)));
            }
        };
    }
}

#endif //MA_DIMENSION_DIMENSION_H
