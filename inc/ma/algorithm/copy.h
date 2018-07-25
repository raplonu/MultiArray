#ifndef MA_COPY_MEM_COPY_H
#define MA_COPY_MEM_COPY_H

#include <ma/traits.h>
#include <ma/function.h>

#include <ma/iterator/StepIterator.h>

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


        template< typename T, typename DST, typename SRC, typename = IsNotSame<T, SRC>, typename... Args >
        void setMem(DST && dst, const SRC & src, SizeT size, Args && ... args)
        {
            copy<T>(forward<DST>(dst), src, size, forward<Args>(args)...);
        }

        template< typename T, typename DST, typename... Args >
        void setMem(DST dst, const T & src, SizeT size, Args && ... args)
        {
            fill<T>( forward<DST>(dst), src, size, forward<Args>(args)... );
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

        template<typename T, typename DST, typename SRC, typename... Args>
        void copyStep(DST && dst, const SRC & src, Args && ... args)
        {
            SizeT size(sizes(dst, src));

            SizeT step(steps(dst, src));

            auto dstIt = iterator::stepIterator(forward<DST>(dst), step);
            auto srcIt = iterator::stepIterator(src, step);

            for(SizeT i(0); i < size; i += step)
            {
                setMem<T>
                (
                    convert<T>(*dstIt),
                    convert<const T>(*srcIt),
                    step, args...
                );

                ++dstIt; ++srcIt;
            }
        }

        template<typename T, typename DST, typename SRC, typename... Args>
        void multiCopy(DST && dst, const SRC & src, Args && ... args)
        {
            if(contiguous(dst) && contiguous(src))
                copyPlain<T>(forward<DST>(dst), src, forward<Args>(args)...);
            else
                copyStep<T>(forward<DST>(dst), src, forward<Args>(args)...);
        }

    }
}

#endif //MA_COPY_MEM_COPY_H
