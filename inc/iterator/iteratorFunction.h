#ifndef ITERATOR_FUNCTION_H
#define ITERATOR_FUNCTION_H

#include "LinearIterator.h"

namespace ma
{
    namespace iterator
    {
        inline SizeT iteratorId(LinearIterator it)
        {
            return *it;
        }

        template<typename T>
        SizeT iteratorId(T it)
        {
            return reinterpret_cast<SizeT>(&(*it));
        }
    }
}

#endif //ITERATOR_FUNCTION_H
