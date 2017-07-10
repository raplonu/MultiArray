#ifndef MA_COPY_MEM_COPY_H
#define MA_COPY_MEM_COPY_H

#include <ma/detail/base.h>

#include <ma/data/containerFunction.h>
#include <ma/iterator/StepIterator.h>

namespace ma
{
    namespace copy
    {

        template<typename DataT, typename DST, typename SRC, typename... Args>
        void memCopyPlain(DST & dst, SRC const & src, Args... args)
        {
            SizeT size(detail::sizes(dst, src));

            data::setMem
            <
                typename detail::get_allocator_type<DST, detail::DefaultAlloc<DataT>>::type,
                typename detail::get_allocator_type<SRC, detail::DefaultAlloc<DataT>>::type
            >
            (
                detail::convert<DataT>(dst), detail::convert<const DataT>(src),
                size, args...
            );
        }

        template<typename DataT, typename DST, typename SRC, typename... Args>
        void memCopyStep(DST & dst, SRC const & src, Args... args)
        {
            SizeT size(detail::sizes(dst, src));

            SizeT step(detail::steps(dst, src));

            auto dstBeginIt = iterator::stepIterator<DataT>(dst, step);
            auto srcBeginIt = iterator::stepIterator<const DataT>(src, step);

            for(SizeT it(0); it < size; it += step)
            {
                data::setMem
                <
                    typename detail::get_allocator_type<DST, detail::DefaultAlloc<DataT>>::type,
                    typename detail::get_allocator_type<SRC, detail::DefaultAlloc<DataT>>::type
                >
                (
                    detail::convert<DataT>(dstBeginIt),
                    detail::convert<const DataT>(srcBeginIt),
                    step, args...
                );

                ++dstBeginIt; ++srcBeginIt;
            }
        }

        template<typename DataT, typename DST, typename SRC, typename... Args>
        void memCopy(DST & dst, SRC const & src, Args... args)
        {
            if(detail::isContigous(dst) && detail::isContigous(src))
                memCopyPlain<DataT>(dst, src, args...);
            else
                memCopyStep<DataT>(dst, src, args...);
        }

    }
}

#endif //MA_COPY_MEM_COPY_H
