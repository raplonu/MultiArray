#ifndef MA_COPY_MEM_COPY_H
#define MA_COPY_MEM_COPY_H

#include <ma_api/traits.h>
#include <ma_api/function.h>

#include <ma_api/iterator/StepIterator.h>

namespace ma
{
    namespace algorithm
    {
        template<typename T>
        void copy(T * dst, const T * src, SizeT size)
        {
            ma::copy_n(src, size, dst);
        }

        template<typename T>
        void fill(T * dst, const T & val, SizeT size)
        {
            ma::fill_n(dst, size, val);
        }

        template<typename, typename, typename> struct ProxyCopy;

        template<typename T>
        struct ProxyCopy<T, T *, const T *>
        {
            static void copy(T * dst, const T * src, SizeT size)
            {
                ma::algorithm::copy(dst, src, size);
            }
        };

        template<typename T>
        struct ProxyCopy<T, T *, const T &>
        {
            static void copy(T * dst, const T & src, SizeT size)
            {
                ma::algorithm::fill(dst, src, size);
            }
        };

        template<typename T, typename DST, typename SRC, typename... Args>
        void setMem(DST && dst, SRC && src, SizeT size, Args && ... args)
        {
            ProxyCopy<T, DST, SRC>::copy
            (
                forward<DST>(dst),
                forward<SRC>(src),
                size, forward<Args>(args)...
            );
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
