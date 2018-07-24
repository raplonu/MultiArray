#ifndef MA_COPY_MEM_COPY_H
#define MA_COPY_MEM_COPY_H

#include <ma/traits.h>
#include <ma/function.h>

namespace ma
{
    namespace algorithm
    {
        template<typename T> //force template property
        void copy(T * dst, T const * src, SizeT size)
        {
            ma::copy_n(src, size, dst);
        }

        template<typename T> //force template property
        void fill(T * dst, T val, SizeT size)
        {
            ma::fill_n(dst, size, val);
        }

        template<typename T, typename DST, typename... Args>
        void setMem(DST dst, T val, SizeT size, Args && ... args)
        {
            fill<T>( dst, val, size, forward<Args>(args)... );
        }

        template
        <
            typename T, typename DST, typename SRC,
            typename = IsNotSame<SRC, T>,
            typename... Args
        >
        void setMem(DST dst, SRC src, SizeT size, Args && ... args)
        {
            copy<T>(dst, src, size, forward<Args>(args)...);
        }

        template<typename T, typename DST, typename SRC, typename... Args>
        void copyPlain(DST && dst, const SRC & src, Args && ... args)
        {
            SizeT size(sizes(dst, src));

            setMem<T>(
                convert<T>(forward<DST>(dst)),
                convert<const T>(src),
                size, forward<Args>(args)...
            );
        }

        // template<typename T, typename DST, typename SRC, typename... Args>
        // void copyStep(DST & dst, SRC const & src, Args... args)
        // {
        //     SizeT size(detail::sizes(dst, src));

        //     SizeT step(detail::steps(dst, src));

        //     auto dstBeginIt = iterator::stepIterator<T>(dst, step);
        //     auto srcBeginIt = iterator::stepIterator<const T>(src, step);

        //     for(SizeT it(0); it < size; it += step)
        //     {
        //         data::setMem<T>
        //         (
        //             detail::convert<T>(dstBeginIt),
        //             detail::convert<const T>(srcBeginIt),
        //             step, args...
        //         );

        //         ++dstBeginIt; ++srcBeginIt;
        //     }
        // }

        template<typename T, typename DST, typename SRC, typename... Args>
        void multiCopy(DST && dst, const SRC & src, Args && ... args)
        {
            if(contiguous(dst) && contiguous(src))
                copyPlain<T>(forward<DST>(dst), src, forward<Args>(args)...);
            // else
            //     copyStep<T>(forward<DST>(dst), src, forward<Args>(args)...);
        }

    }
}

#endif //MA_COPY_MEM_COPY_H
