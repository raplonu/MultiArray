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
        template
        <
            typename T, typename AllocatorDst, typename AllocatorSrc,
            typename Enabled = void
        >
        struct MemCopy;

        template
        <
            typename T, typename Alloc,
            typename Enabled = void
        >
        struct MemFill;

        template<typename T>
        struct MemCopy<T, detail::DefaultAlloc<T>, detail::DefaultAlloc<T>>
        {
            static void copy(T * dst, T const * src, SizeT size)
            {
                std::memcpy(dst, src, size * sizeof(T));
            }
        };

        template<typename T>
        struct MemFill<T, detail::DefaultAlloc<T>>
        {
            static void fill(T * dst, T val, SizeT size)
            {
                std::fill_n(dst, size, val);
            }
        };

        template<
            typename AllocatorDst, typename AllocatorSrc,
            typename T, typename... Args>
        void setMem(T * dst, T val, SizeT size, Args... args)
        {
            MemFill<T, AllocatorDst>::fill(
                dst, val, size, args...
            );
        }

        template
        <
            typename AllocatorDst, typename AllocatorSrc,
            typename T, typename... Args
        >
        void setMem(T * dst, T const * src, SizeT size, Args... args)
        {
            MemCopy<T, AllocatorDst, AllocatorSrc>::copy(
                dst, src, size, args...
            );
        }

    }
}

#endif //MA_DATA_CONTAINER_FUNCTION_H
