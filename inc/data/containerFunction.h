#ifndef MA_CONTAINER_CONATAINER_FUNCTION_H
#define MA_CONTAINER_CONATAINER_FUNCTION_H

#include <algorithm> //std::fill
#include <cstring>   //std::memcpy

namespace ma
{
    namespace container
    {
        template<typename AllocSrc, typename AllocDst>
        struct MemCopy;

        template<typename Alloc>
        struct MemFill;

        template<typename T>
        struct MemCopy<DefaultAlloc<T>, DefaultAlloc<T>>
        {
            template<typename = void> //force template property
            static void copy(T * dst, T const * src, SizeT length)
            {
                std::memcpy(dst, src, length * sizeof(T));
            }
        };

        template<typename T>
        struct MemFill<DefaultAlloc<T>>
        {
            template<typename = void> //force template property
            static void fill(T * dst, T val, SizeT size)
            {
                std::fill_n(dst, size, val);
            }
        };

        template<typename AllocatorDst, typename AllocatorSrc, typename T, typename... Args>
        void setMem(T * dst, T val, SizeT size, Args... args)
        {
            MemFill<AllocatorDst>::template fill<Args...>(
                dst, val, size, args...
            );
        }

        template<typename AllocatorDst, typename AllocatorSrc, typename T, typename... Args>
        void setMem(T * dst, T const * src, SizeT size, Args... args)
        {
            MemCopy<AllocatorDst, AllocatorSrc>::template copy<Args...>(
                dst, src, size, args...
            );
        }

    }
}

#endif //MA_CONTAINER_CONATAINER_FUNCTION_H
