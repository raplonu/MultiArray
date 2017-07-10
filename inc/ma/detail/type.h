#ifndef MA_DETAIL_TYPE_H
#define MA_DETAIL_TYPE_H

#include <cstdint>
#include <memory>
#include <vector>

namespace ma
{
    namespace detail
    {
        struct SuperBase{};


        using SizeT = std::size_t;
        using DiffT = std::ptrdiff_t;

        // struct ShapeConstructT{};
        // struct DataConstructT{};
        // struct ShapeDataConstructT{};

        struct All{};
        struct Delay{};

        // static ShapeConstructT shapeConstruct;
        // static DataConstructT dataConstruct;
        // static ShapeDataConstructT shapeDataConstruct;

        template<typename T>
        using DefaultAlloc = std::allocator<T>;

        template<typename T, typename Alloc = DefaultAlloc<T>>
        using DefaultContainer = std::vector<T, Alloc>;

    }

    using detail::SizeT;
    using detail::DiffT;

    static detail::Delay delay;
}

#endif //MA_DETAIL_TYPE_H
