#ifndef MA_DIMENSION_MULTI_SHAPE_H
#define MA_DIMENSION_MULTI_SHAPE_H

#include <ma/type.h>
#include <ma/function.h>

#include <ma/dimension/Dimension.h>
#include <ma/dimension/dimensionFunction.h>

namespace ma
{
    namespace dimension
    {
        template<typename Range>
        class MultiShape
        {
        public:
            using DimensionT = Dimension<Range>;
        protected:
            VectDimension<Range> dims_;

        public:
            constexpr explicit MultiShape() noexcept = default;

            template<typename L>
            explicit MultiShape(L && lengths):
                dims_(initVectDim<Range>(forward<L>(lengths)))
            {}

            template<typename L, std::size_t N >
            explicit MultiShape(const L (&lengths)[N]):
                dims_(initVectDim<Range>(lengths))
            {}

            explicit MultiShape(const BasicShape & b):
                dims_(1, Range(b.start(), b.stop()))
            {}


        protected:
            MultiShape(VectDimension<Range> && dims):
                dims_(move(dims))
            {}

        public:
            MultiShape(const MultiShape &) = default;
            constexpr MultiShape(MultiShape &&) noexcept = default;

            MultiShape& operator=(const MultiShape &) = default;
            MultiShape& operator=(MultiShape &&) noexcept = default;

            template<typename... R>
            MultiShape subShape(R && ... ranges) const
            {
                return MultiShape(selectDimensions(dims_, forward<R>(ranges)...));
            }

            MultiShape closeAt(SizeT pos) const
            {
                return MultiShape(selectDimensions(dims_, pos));
            }

            
            SizeT at(SizeT pos) const noexcept
            {
                SizeT
                    off(0), dim(0),
                    restSize(size());

                while(pos != 0)
                {
                    restSize /= sizeAt(dim);
                    off = off * baseSizeAt(dim) + dims_[dim][pos / restSize];
                    pos %= restSize;
                    ++dim;
                }

                for(; dim < dimNb(); ++dim)
                    off = off * baseSizeAt(dim) + dims_[dim][0];

                return off;
            }

            SizeT size() const
            {
                return prodDim([](SizeT s, const DimensionT & d){return s * ma::max<SizeT>(d.size(), 1);});
            }

            SizeT baseSize() const
            {
                return prodDim([](SizeT s, const DimensionT & d){return s * d.baseSize();});
            }

            SizeT step() const
            {
                return contiguousDataLength();
            }

            bool contigous() const
            {
                return contiguousDataLength() == size();
            }

            constexpr SizeT dimNb() const noexcept
            {
                return dims_.size();
            }

            SizeT activeDimNb() const
            {
                return std::count_if(dims_.begin(), dims_.end(),
                    [](const DimensionT & d){return d.active();});
            }

            template<typename Fn>
            SizeT prodDim(Fn fn) const
            {
                return dims_.empty()
                    ? 0
                    : accumulate(dims_.begin(), dims_.end(), 1, fn);
            }

            bool activeAt(SizeT dim) const
            {
                return dims_[dim].active();
            }

            SizeT sizeAt(SizeT dim) const
            {
                return ma::max<SizeT>(dims_[dim].size(), 1);
            }

            SizeT baseSizeAt(SizeT dim) const
            {
                return dims_[dim].baseSize();
            }

            SizeT rangedElementAtNb(SizeT dim) const
            {
                return dims_[dim].rangedElementNb();
            }

            SizeT baseOffset() const
            {
                return at(0);
            }

            SizeT contiguousDataLength(SizeT lastDim = 0) const
            {
                SizeT cdl(1), dim(dims_.size());

                while((--dim > lastDim) && dims_[dim].complete())
                    cdl *= sizeAt(dim);

                return cdl * rangedElementAtNb(dim);
            }

            // SizeT firstActiveDim() const
            // {
            //     for(SizeT dim(0); dim < nbDim(); ++dim)
            //         if(isActiveDim(dim))
            //             return dim;
            //     return 0;
            // }


            // SizeT nbCompleteDim() const
            // {
            //     SizeT nb(0);
            //     for(auto const & e : dims_)
            //         if(e.isComplete())
            //             ++nb;
            //     return nb;
            // }

            // SizeT nbRangedDim() const
            // {
            //     SizeT nb(0), dim(dims_.size());
            //     while((dim-- > 0) && dims_[dim].isComplete())
            //         ++nb;
            //     return nb;
            // }

            // SizeT nbNotRangedDim() const
            // {
            //     return dims_.size() - nbRangedDim();
            // }

            // SizeVect baseShape(SizeT nbDim) const
            // {
            //     SizeVect shape(nbDim);

            //     std::transform(
            //         dims_.begin(), dims_.begin() + nbDim, shape.begin(),
            //         [](DimensionT const &dim){return dim.size();}
            //     );

            //     return shape;
            // }

            // SizeVect baseShape() const
            // {
            //     return baseShape(nbDim());
            // }

            // SizeVect shape(SizeT nbDimMax) const
            // {
            //     SizeVect shape(nbDimMax);

            //     SizeT activeDim(0);
            //     for(SizeT dim(0); activeDim < nbDimMax && dim < nbDim(); ++dim)
            //         if(dims_[dim].isActive())
            //             shape[activeDim++] = dims_[dim].size();

            //     return shape;
            // }

            // SizeVect shape() const
            // {
            //     return shape(nbActiveDim());
            // }


            // //dimNb, dimStep
            // std::pair<SizeT, SizeT> getDim(SizeT step) const
            // {
            //     SizeT dim(dims_.size()), lastStep;

            //     do
            //     {
            //         step = (lastStep = step) / sizeAt(--dim);
            //     }
            //     while(step != 0 && dim != 0);

            //     if((sizeAt(dim) % lastStep) != 0)
            //         throw std::length_error("Size doesn't fit shape");

            //     return std::make_pair(dim + 1, lastStep);
            // }
        };
    }
}

#endif //MA_DIMENSION_MULTI_SHAPE_H
