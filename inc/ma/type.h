#ifndef MA_TYPE_H
#define MA_TYPE_H

#include <cstddef>      // size_t & ptrdiff_t
#include <cstdint>      // size_t & ptrdiff_t
#include <memory>       // allocator

#include <vector>       // vector
#include <array>        // array

namespace ma
{
    using SizeT = std::ptrdiff_t;
    using DiffT = std::ptrdiff_t;

    // TODO define HDiffT to be half size of DiffT
    using HDiffT = std::int16_t;

    enum All{all, aut};
    enum Delay{delay, pass};

    template<typename T>
    using DefaultAlloc = std::allocator<T>;

    using VectRange = std::vector<SizeT>;

    template<SizeT N>
    using ArrayRange = std::array<SizeT, N>;
}

#endif //MA_TYPE_H
