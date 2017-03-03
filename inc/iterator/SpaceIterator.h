#ifndef MA_ITERATOR_SPACE_ITERATOR_H
#define MA_ITERATOR_SPACE_ITERATOR_H

#include "../space/Space.h"

#include <vector>
#include <algorithm>

namespace ma
{
    namespace iterator
    {
        template<typename T>
        class SpaceIterator
        {
            using RangeT = T;
            using SpaceIteratorT = SpaceIterator<T>;
            using SpaceT = space::Space<T>;
            using PosVectT = std::vector<SizeT>;

            SpaceT const & space_;
            PosVectT posVect_;
            SizeT step_;
            SizeT contiguousDataLength_;

        public:
            SpaceIterator(SpaceT const & space):
                space_(space),
                posVect_(std::max(space.nbNotRangedDim(), 1ul), 0),
                step_(space.nbRangedElementAt(posVect_.size() - 1)),
                contiguousDataLength_(space.contiguousDataLength(space.nbNotRangedDim()))
            {}

            SpaceIterator(SpaceT const & space, SizeT nbDim, SizeT step):
                space_(space), posVect_(space.baseShape(std::max(nbDim, 1ul))),
                step_(step), contiguousDataLength_(space.contiguousDataLength(nbDim))
            {}

            SizeT operator*() const
            {
                return space_.offsetAt(posVect_);
            }

            SpaceIterator& operator++()
            {
                increment();
                return *this;
            }

            void increment()
            {
                SizeT dim(posVect_.size() - 1);
                posVect_[dim] += step_;

                while(!isDimvalid(dim) && dim > 0)
                {
                    posVect_[dim] = 0;
                    --dim;
                    posVect_[dim]++;
                }
            }

            bool isDimvalid(SizeT dim) const
            {
                return posVect_[dim] < std::max(space_.sizeAt(dim), 1ul);
            }

            bool finish() const
            {
                return !isDimvalid(0);
            }

            SizeT contiguousDataLength() const
            {
                return contiguousDataLength_;
            }
        };
    }
}

#endif //MA_ITERATOR_SPACE_ITERATOR_H
