#ifndef MA_ITERATOR_ITERATOR_FUNCTION_H
#define MA_ITERATOR_ITERATOR_FUNCTION_H

#include <ma/type.h>
#include <ma/iterator/LinearIterator.h>

namespace ma
{
    namespace iterator
    {
        constexpr inline SizeT iteratorId(LinearIterator it) noexcept
        {
            return *it;
        }


        template<typename T, typename = IsNotLinearIterator<T>>
        constexpr SizeT iteratorId(const T & it) noexcept
        {
            //access address as ID to compare 2 iterator
            return reinterpret_cast<SizeT>(&(*it));
        }
    }
}

#endif //MA_ITERATOR_ITERATOR_FUNCTION_H
