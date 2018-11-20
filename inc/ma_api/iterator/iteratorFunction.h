#ifndef MA_ITERATOR_ITERATOR_FUNCTION_H
#define MA_ITERATOR_ITERATOR_FUNCTION_H

#include <ma_api/type.h>
#include <ma_api/iterator/LinearIterator.h>

namespace ma
{
    namespace iterator
    {
        constexpr inline int iteratorId(LinearIterator it) noexcept
        {
            return *it;
        }


        template<typename T, typename = IsNotLinearIterator<T>>
        constexpr std::ptrdiff_t iteratorId(const T & it) noexcept
        {
            //access address as ID to compare 2 iterator
            return reinterpret_cast<std::ptrdiff_t>(&(*it));
        }
    }
}

#endif //MA_ITERATOR_ITERATOR_FUNCTION_H
