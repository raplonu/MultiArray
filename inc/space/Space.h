#ifndef SAPCE_H
#define SAPCE_H

#include <iostream>
#include <memory>
#include <vector>
#include <tuple>

#include "../common/function.h"
#include "../range/Range.h"
#include "../dimension/Dimension.h"

namespace ma
{
    namespace space
    {
        template<typename T>
        class Space
        {
            using RangeT = T;
            using SpaceT = Space<T>;
            using Dim = dimension::Dimension<T>;
            using DimVect = std::vector<dimension::Dimension<T>>;
            using SizeVect = std::vector<SizeT>;

            DimVect dims_;

        public:
            Space():dims_(){}

            Space(DimVect && dims):dims_(std::move(dims)){}

            Space(SpaceT const & oSpace):dims_(oSpace.dims_){}
            Space(SpaceT && oSpace):dims_(std::move(oSpace.dims_)){}

            template<typename L>
            Space(L && lengths):dims_(dimension::initDimVect<Dim>(lengths)){}

            template<typename L, SizeT NbDim>
            Space(L const (&lengths)[NbDim]):dims_(dimension::initDimVect<Dim>(lengths)){}

            SpaceT& operator=(SpaceT const & oSpace) = default;
            SpaceT& operator=(SpaceT && oSpace) = default;

            ~Space(){}

            SizeT nbDim() const
            {
                return dims_.size();
            }

            SizeT size()
            {
                return prodSize([](auto const & e){return std::max(e.size(), 1ul);});
            }

            SizeT realSize()
            {
                return prodSize([](auto const & e){return e.realSize();});
            }

            SizeT prodSize(auto sizeF)
            {
                if(nbDim() == 0)
                    return 0;

                SizeT size(1);
                for(auto const & e : dims_)
                    size *= sizeF(e);

                return size;
            }

            SpaceT operator[](SizeT pos) const
            {
                DimVect newDims(dims_);
                SizeT dim(firstActiveDim());

                newDims[dim] = newDims[dim].closeAt(pos);

                return {std::move(newDims)};
            }

            SpaceT at(DimVect dims) const
            {
                DimVect newDims(nbDim());

                SizeT d1(0), d2(0), s1(nbActiveDim()), s2(dims.size());

                while(d1 < s1 && d2 < s2)
                {
                    if(isActiveDim(d1))
                        newDims[d1] = dims_[d1].select(dims[d2++]);
                    else
                        newDims[d1] = dims_[d1];

                    ++d1;
                }

                return {std::move(newDims)};
            }

            SizeT firstActiveDim() const
            {
                for(SizeT dim(0); dim < nbDim(); ++dim)
                    if(isActiveDim(dim))
                        return dim;
                return 0;
            }

            bool isActiveDim(SizeT dim) const
            {
                return dims_[dim].isActive();
            }

            SizeT sizeAt(SizeT dim) const
            {
                return dims_[dim].size();
            }

            SizeT nbRangedElementAt(SizeT dim) const
            {
                return dims_[dim].nbRangedElement();
            }

            SizeT offsetAt(SizeVect const & posVect) const
            {
                SizeT res(0), dim(0);

                for(;dim < posVect.size(); ++dim)
                    res = (res * dims_[dim].realSize()) + dims_[dim][posVect[dim]];

                for(;dim < nbDim(); ++dim)
                    res = (res * dims_[dim].realSize()) + dims_[dim][0];

                return res;
            }

            SizeT baseOffset() const
            {
                SizeT off(0);

                for(SizeT dim(0);dim < nbDim(); ++dim)
                    off = (off * dims_[dim].realSize()) + dims_[dim][0];

                return off;
            }

            SizeT nbActiveDim() const
            {
                SizeT nb(0);
                for(auto const & e : dims_)
                    if(e.isActive())
                        ++nb;
                return nb;
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
                    [](Dim const &dim){return dim.size();}
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
        };
    }
}

#endif //SAPCE_H
