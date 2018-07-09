#ifndef MA_TYPE_H
#define MA_TYPE_H

#include <cstddef> //size_t & ptrdiff_t
#include <memory> // allocator

#include <vector>

namespace ma
{
    using SizeT = std::ptrdiff_t;
    using DiffT = std::ptrdiff_t;

    enum All{all, aut};
    enum Delay{delay, pass};

    template<typename T>
    using DefaultAlloc = std::allocator<T>;

    using VectRange = std::vector<SizeT>;
}

#endif //MA_TYPE_H
