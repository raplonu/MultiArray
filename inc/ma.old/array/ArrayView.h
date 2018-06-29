#ifndef MA_ARRAY_ARRAY_VIEW_H
#define MA_ARRAY_ARRAY_VIEW_H

namespace ma
{
    template<typename T, typename Ref, typename Shape>
    class ArrayView
    {
    protected:
        Ref ref_;
        Shape shape_;

        ArrayView() = default;

        
    };
}

#endif //MA_ARRAY_ARRAY_VIEW_H