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

        enum All_{All, Auto};
        enum Delay_{Delay};

        template<typename T>
        using DefaultAlloc = std::allocator<T>;

        template<typename T, typename Alloc = DefaultAlloc<T>>
        using DefaultContainer = std::vector<T, Alloc>;

    }

    using detail::SizeT;
    using detail::DiffT;

    using detail::All;
    using detail::Auto;
    using detail::Delay;

    // static detail::All All;
    // static detail::All Auto;
    // static detail::Delay delay;
}

#endif //MA_DETAIL_TYPE_H
