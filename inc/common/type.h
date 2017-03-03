#ifndef TYPE_H
#define TYPE_H

#include <cstdint>

namespace ma
{
    using SizeT = std::size_t;
    using DiffT = std::ptrdiff_t;

    template<typename T>
    using DefaultAlloc = std::allocator<T>;
}

#endif //TYPE_H
