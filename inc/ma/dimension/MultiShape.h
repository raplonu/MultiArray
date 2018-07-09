#ifndef MA_DIMENSION_MULTI_SHAPE_H
#define MA_DIMENSION_MULTI_SHAPE_H

#include <ma/type.h>
#include <ma/dimension/Dimension.h>

namespace ma
{
    namespace dimension
    {
        template<typename Range>
        class MultiShape
        {
        protected:
            std::vector<Dimension<Range>> dims_;

        public:
            constexpr explicit MultiShape() noexcept = default;

            template<typename L, typename = IsIterable<L>>
            explicit MultiShape(L && lengths):
                dims_(initVectDim<Range>(lengths))
            {}

            template
            <
                typename L,
                typename = enable_if_t<!std::is_base_of<MultiShape, L>::value>::type
            >
            MultiShape(L && lengths):
                dims_(dimension::initDimVect<DimT>(lengths)), offset_(0)
            {}

            template<typename L>
            MultiShape(std::initializer_list<L> const & lengths, SizeT offset = 0):
                dims_(dimension::initDimVect<DimT>(lengths)), offset_(offset)
            {}

            MultiShape(SimpleShape const & sShape):
                dims_(1, RangeT(sShape.start(), sShape.stop())), offset_(0)
            {}

        protected:
            MultiShape(DimVect && dims, SizeT offset):
                dims_(std::move(dims)), offset_(offset)
            {}

        public:
            MultiShape(const MultiShape &) = default;
            MultiShape(MultiShape &&) = default;

            MultiShape& operator=(const MultiShape &) = default;
            MultiShape& operator=(MultiShape &&) = default;

            SizeT at(SizeT pos) const
            {
                if(pos > size())
                    throw std::out_of_range("Error!!!!!");

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

                for(; dim < nbDim(); ++dim)
                    off = off * baseSizeAt(dim) + dims_[dim][0];

                return offset_ + off;
            }

            template<typename... R>
            MultiShape subShape(R&&... ranges) const
            {
                return MultiShape(selectDimensions(dims_, ranges...), offset_);
            }

            SizeT size() const
            {
                return prodSize([](DimT const & d){return std::max(d.size(), 1ul);});
            }

            SizeT baseSize() const
            {
                return prodSize([](DimT const & e){return e.baseSize();});
            }

            SizeT step() const
            {
                return contiguousDataLength();
            }

            bool isContigous() const
            {
                return contiguousDataLength() == size();
            }

            SizeT nbDim() const
            {
                return dims_.size();
            }

            SizeT nbActiveDim() const
            {
                SizeT nb(0);
                for(auto const & e : dims_)
                    if(e.isActive())
                        ++nb;
                return nb;
            }

            template<typename Fn>
            SizeT prodSize(Fn sizeF) const
            {
                if(nbDim() == 0)
                    return 0;

                SizeT size(1);
                for(auto const & e : dims_)
                    size *= sizeF(e);

                return size;
            }

            // SizeT firstActiveDim() const
            // {
            //     for(SizeT dim(0); dim < nbDim(); ++dim)
            //         if(isActiveDim(dim))
            //             return dim;
            //     return 0;
            // }

            bool isActiveDim(SizeT dim) const
            {
                return dims_[dim].isActive();
            }

            SizeT sizeAt(SizeT dim) const
            {
                return std::max(dims_[dim].size(), 1ul);
            }

            SizeT baseSizeAt(SizeT dim) const
            {
                return dims_[dim].baseSize();
            }

            SizeT nbRangedElementAt(SizeT dim) const
            {
                return dims_[dim].nbRangedElement();
            }

            SizeT baseOffset() const
            {
                return at(0);
            }



            SizeT nbCompleteDim() const
            {
                SizeT nb(0);
                for(auto const & e : dims_)
                    if(e.isComplete())
                        ++nb;
                return nb;
            }

            SizeT nbRangedDim() const
            {
                SizeT nb(0), dim(dims_.size());
                while((dim-- > 0) && dims_[dim].isComplete())
                    ++nb;
                return nb;
            }

            SizeT nbNotRangedDim() const
            {
                return dims_.size() - nbRangedDim();
            }

            SizeVect baseShape(SizeT nbDim) const
            {
                SizeVect shape(nbDim);

                std::transform(
                    dims_.begin(), dims_.begin() + nbDim, shape.begin(),
                    [](DimT const &dim){return dim.size();}
                );

                return shape;
            }

            SizeVect baseShape() const
            {
                return baseShape(nbDim());
            }

            SizeVect shape(SizeT nbDimMax) const
            {
                SizeVect shape(nbDimMax);

                SizeT activeDim(0);
                for(SizeT dim(0); activeDim < nbDimMax && dim < nbDim(); ++dim)
                    if(dims_[dim].isActive())
                        shape[activeDim++] = dims_[dim].size();

                return shape;
            }

            SizeVect shape() const
            {
                return shape(nbActiveDim());
            }

            SizeT contiguousDataLength(SizeT lastDim = 0) const
            {
                SizeT cdl(1), dim(dims_.size());

                while((--dim > lastDim) && dims_[dim].isComplete())
                    cdl *= sizeAt(dim);

                return cdl * nbRangedElementAt(dim);
            }

            //dimNb, dimStep
            std::pair<SizeT, SizeT> getDim(SizeT step) const
            {
                SizeT dim(dims_.size()), lastStep;

                do
                {
                    step = (lastStep = step) / sizeAt(--dim);
                }
                while(step != 0 && dim != 0);

                if((sizeAt(dim) % lastStep) != 0)
                    throw std::length_error("Size doesn't fit shape");

                return std::make_pair(dim + 1, lastStep);
            }
        };
    }
}

#endif //MA_DIMENSION_MULTI_SHAPE_H
