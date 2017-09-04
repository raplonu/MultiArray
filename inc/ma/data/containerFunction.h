#ifndef MA_DATA_CONTAINER_FUNCTION_H
#define MA_DATA_CONTAINER_FUNCTION_H

#include <algorithm> //std::fill
#include <iostream> //std::fill
#include <cstring>   //std::memcpy

#include <ma/detail/base.h>

namespace ma
{
    namespace data
    {
        // template
        // <
        //     typename T
        //     // , typename AllocatorDst, typename AllocatorSrc,
        //     // typename Enabled = void
        // >
        // struct MemCopy;

        // template
        // <
        //     typename T
        //     // , typename Alloc,
        //     // typename Enabled = void
        // >
        // struct MemFill;

        // template<typename T>
        // struct MemCopy//<T, detail::DefaultAlloc<T>, detail::DefaultAlloc<T>>
        // {
            template<typename T> //force template property
            void copy(T * dst, T const * src, SizeT size)
            {
                std::memcpy(dst, src, size * sizeof(T));
            }
        // };

        // template<typename T>
        // struct MemFill//<T, detail::DefaultAlloc<T>>
        // {
            template<typename T> //force template property
            void fill(T * dst, T val, SizeT size)
            {
                std::fill_n(dst, size, val);
            }
        // };
        // template<typename T, typename DST> //force template property
        // void fill(DST dst, T val, SizeT size)
        // {
        //     //thrust::fill_n(dst, size, val);
        // }

        template<
            //typename AllocatorDst, typename AllocatorSrc,
            typename T, typename DST, typename... Args>
        void setMem(DST dst, T val, SizeT size, Args... args)
        {
            //MemFill<T/*, AllocatorDst*/>::template
            fill<T>(
                dst, val, size, args...
            );
        }

        template
        <
            //typename AllocatorDst, typename AllocatorSrc,
            typename T, typename DST, typename SRC, typename = typename std::enable_if<!std::is_same<SRC, T>::value>::type, typename... Args
        >
        void setMem(DST dst, SRC src, SizeT size, Args... args)
        {
            //MemCopy<T/*, AllocatorDst, AllocatorSrc*/>::template
            copy<T>(
                dst, src, size, args...
            );
        }


    }
}

#endif //MA_DATA_CONTAINER_FUNCTION_H
