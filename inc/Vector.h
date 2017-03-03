#ifndef MA_VECTOR_H
#define MA_VECTOR_H

namespace ma
{
    template<typename T, typename Allocator, typename Range>
    class BaseVector
    {

    };

    template<typename T, typename Allocator = DefaultAlloc>
    using Vector = BaseVector<T, Allocator, range::LinearRange>;

    template<typename T, typename Allocator = DefaultAlloc>
    using SVector = BaseVector<T, Allocator, range::Range>;
}

#endif //MA_VECTOR_H
