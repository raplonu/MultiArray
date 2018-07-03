#ifndef MA_ITERATOR_ITERATOR_FUNCTION_H
#define MA_ITERATOR_ITERATOR_FUNCTION_H

#include <ma/type.h>
#include <ma/iterator/LinearIterator.h>

namespace ma
{
    namespace iterator
    {
        inline SizeT iteratorId(LinearIterator it)
        {
            return *it;
        }

        template<typename T>
        using IsNotLinearIterator = enable_if_t<!is_same<T, LinearIterator>::value>;

        template<typename T, typename = IsNotLinearIterator<T>>
        SizeT iteratorId(const T & it)
        {
            //access address as ID to compare 2 iterator
            return reinterpret_cast<SizeT>(&(*it));
        }
    }
}

#endif //MA_ITERATOR_ITERATOR_FUNCTION_H
