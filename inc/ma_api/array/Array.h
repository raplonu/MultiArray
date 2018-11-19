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

#ifndef MA_ARRAY_ARRAY_H
#define MA_ARRAY_ARRAY_H

#include <ma_api/array/ArrayView.h>

/// MultiArray namespace
namespace ma {
    /// array namespace
    namespace array {
        /**
         * \addtogroup View
         * @{
         */

        /**
         * @brief The Array class consist to a the association of a container, an allocator and a shape for allocate, store and access array
         * 
         * @tparam T The array element type
         * @tparam Container The store array type
         * @tparam Shape The shape type
         * 
         * \par Overview
         */
        template <typename T, typename Container, typename Shape>
        class Array : public ArrayView<T, typename Container::allocator_type, Shape>
        {
            using View = ArrayView<T, typename Container::allocator_type, Shape>;
        public:
            using value_type = T;
            using allocator_type = typename Container::allocator_type;

        protected:
            Container container_;

            template<typename L>
            constexpr Array(Container && container, L && l) noexcept(noexcept(View(std::forward<L>(l), ptrOf(container)))) :
                View(std::forward<L>(l), ma::ptrOf(container)), container_(std::move(container))
            {}

        public:
            constexpr explicit Array(const allocator_type& allocator = allocator_type()) noexcept :
                View(), container_(allocator)
            {}

            template<typename L, typename = IsNotEquivalent<L, allocator_type>, typename = IsNotEquivalent<L, View>, typename = IsNotEquivalent<L, Array>>
            explicit Array(L && l, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), forward<L>(l))
            {}

            template<typename L>
            explicit Array(const initializer_list<L> & l, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), l)
            {}

            template<typename L, typename Data, typename = IsNotEquivalent<Data, Array>, typename = IsNotEquivalent<L, Container>, typename = IsNotEquivalent<Data, allocator_type>>
            explicit Array(L && l, Data && data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), forward<L>(l))
            {
                View::setMem(std::forward<Data>(data));
            }

            template<typename L, typename Data, typename = IsNotEquivalent<Data, allocator_type>>
            explicit Array(const initializer_list<L> & l, Data && data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), l)
            {
                View::setMem(std::forward<Data>(data));
            }

            template<typename L, typename Data, typename = IsNotEquivalent<L, Container>>
            explicit Array(L && l, const initializer_list<Data> & data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), forward<L>(l))
            {
                View::setMem(data);
            }

            template<typename L, typename Data>
            explicit Array(const initializer_list<L> & l, const initializer_list<Data> & data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), l)
            {
                View::setMem(data);
            }

            explicit Array(const View & view, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(view.shape()), allocator), view.shape())
            {
                View::setMem(view);
            }

            Array(const Array &) = default;
            constexpr Array(Array &&) noexcept = default;

            Array& operator=(const Array &) = default;
            Array& operator=(Array &&) = default;

            constexpr const allocator_type & get_allocator() const
            {
                return container_.get_allocator();
            }

            constexpr SizeT baseSize() const
            {
                return container_.size();
            }

            using View::size;
        };
        /** @} */       // end group View
    }
}

#endif //MA_ARRAY_ARRAY_H