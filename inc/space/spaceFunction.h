#ifndef MA_SPACE_SPACE_FUNCTION_H
#define MA_SPACE_SPACE_FUNCTION_H

#include <algorithm>
#include <vector>

namespace ma
{
    namespace space
    {
        template<typename T>
        bool shapeEqual(std::vector<T> const & v1, std::vector<T> const & v2)
        {
            return std::equal(v1.cbegin(), v1.cend(), v2.cbegin());
        }
    }
}

#endif //MA_SPACE_SPACE_FUNCTION_H
