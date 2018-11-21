/**********************************************************************************
 * Copyright (c) 2016-2019 Julien Bernard, https://github.com/raplonu/MultiArray
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************/
#ifndef MA_COPY_MEM_COPY_H
#define MA_COPY_MEM_COPY_H

#include <ma_api/traits.h>
#include <ma_api/function.h>

#include <ma_api/iterator/StepIterator.h>

namespace ma
{
    namespace algorithm
    {
        /**
         * @brief copy data from buffer to another
         * 
         * @tparam T the underlying type
         * @param dst address of destination buffer
         * @param src address of source buffer
         * @param size number of element to copy
         */
        template<typename T>
        void copy(T * dst, const T * src, int size)
        {
            ma::copy_n(src, size, dst);
        }

        /**
         * @brief fill a buffer this the same element
         * 
         * @tparam T the underlying type
         * @param dst address of destination buffer
         * @param val the value used to fill buffer
         * @param size number of element to fill
         */
        template<typename T>
        void fill(T * dst, const T & val, int size)
        {
            ma::fill_n(dst, size, val);
        }

        template< typename, typename, typename> struct ProxyCopy;

        /**
         * @brief Proxy that is used for raw pointer data copy
         * 
         * @tparam T the underlying type
         */
        template<typename T>
        struct ProxyCopy<T, T *, const T *>
        {
            /**
             * @brief copy data from buffer to another
             * 
             * @param dst address of destination buffer
             * @param src address of source buffer
             * @param size number of element to copy
             */
            static void copy(T * dst, const T * src, int size)
            {
                ma::algorithm::copy(dst, src, size);
            }
        };

        /**
         * @brief Proxy that is used for raw pointer data fill
         * 
         * @tparam T the underlying type
         */
        template<typename T>
        struct ProxyCopy<T, T *, const T &>
        {
            /**
             * @brief fill a buffer this the same element
             * 
             * @param dst address of destination buffer
             * @param val the value used to fill buffer
             * @param size number of element to fill
             */
            static void copy(T * dst, const T & src, int size)
            {
                ma::algorithm::fill(dst, src, size);
            }
        };

        /**
         * @brief select the corresponding proxy depending the SRC and DST data type
         * 
         * @tparam T the underlying type
         * @tparam DST the reference to destination memory type
         * @tparam SRC the reference to source memory to copy or value used to fill destination type
         * @tparam Args additional optional arguments to perform the operation type
         * @param dst the reference to destination memory
         * @param src the reference to source memory to copy or value used to fill destination
         * @param size the number of element in destination buffer
         * @param args additional optional arguments to perform the operation
         */
        template<typename T, typename DST, typename SRC, typename... Args>
        void setMem(DST && dst, SRC && src, int size, Args && ... args)
        {
            ProxyCopy<T, DST, SRC>::copy
            (
                forward<DST>(dst),
                forward<SRC>(src),
                size, forward<Args>(args)...
            );
        }

        /**
         * @brief perform a copy or fill operation on a contiguous memory segment
         * 
         * @tparam T the underlying type
         * @tparam DST the reference to destination memory type
         * @tparam SRC the reference to source memory to copy or value used to fill destination type
         * @tparam Args additional optional arguments to perform the operation type
         * @param dst the reference to destination memory
         * @param src the reference to source memory to copy or value used to fill destination
         * @param args additional optional arguments to perform the operation
         */
        template<typename T, typename DST, typename SRC, typename... Args>
        void copyPlain(DST && dst, const SRC & src, Args && ... args)
        {
            int size(sizes(dst, src));

            setMem<T>(
                convert<T>(forward<DST>(dst)),
                convert<const T &>(src),
                size, forward<Args>(args)...
            );
        }

        /**
         * @brief perform a copy or fill operation on a non contiguous memory segment
         * 
         * @tparam T the underlying type
         * @tparam DST the reference to destination memory type
         * @tparam SRC the reference to source memory to copy or value used to fill destination type
         * @tparam Args additional optional arguments to perform the operation type
         * @param dst the reference to destination memory
         * @param src the reference to source memory to copy or value used to fill destination
         * @param args additional optional arguments to perform the operation
         */
        template<typename T, typename DST, typename SRC, typename... Args>
        void copyStep(DST && dst, const SRC & src, Args && ... args)
        {
            // try get the sizes of dst and src and compare if possible
            int size(sizes(dst, src));

            // try get the steps of dst and src and find the bigger common step
            int step(steps(dst, src));

            // Create iterator for src and dst that move step elements at each iteration
            auto dstIt = iterator::stepIterator(forward<DST>(dst), step);
            auto srcIt = iterator::stepIterator(src, step);

            for(int i(0); i < size; i += step)
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

        /**
         * @brief Perform a minimal number of fill or copy operation depending of the SRC and DST properties
         * 
         * @tparam T the underlying type
         * @tparam DST the reference to destination memory type
         * @tparam SRC the reference to source memory to copy or value used to fill destination type
         * @tparam Args additional optional arguments to perform the operation type
         * @param dst the reference to destination memory
         * @param src the reference to source memory to copy or value used to fill destination
         * @param args additional optional arguments to perform the operation
         */
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
