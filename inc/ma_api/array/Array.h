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
            /**
             * @brief Base class of array, handle data access and modification
             * 
             */
            using View = ArrayView<T, typename Container::allocator_type, Shape>;
        public:
            /**
             * @brief The underlying type
             * 
             */
            using value_type = T;
            /**
             * @brief The allocator type
             * 
             */
            using allocator_type = typename Container::allocator_type;

        protected:
            /**
             * @brief The container member that old the data
             * 
             */
            Container container_;

            /**
             * @brief protected constructor that take a container and a shape
             * 
             * @tparam L the shape type of the constructor
             * @param container the container that old the data
             * @param l the shape of the data
             */
            template<typename L>
            constexpr Array(Container && container, L && l) noexcept(noexcept(View(std::forward<L>(l), ptrOf(container)))) :
                View(std::forward<L>(l), ma::ptrOf(container)), container_(std::move(container))
            {}

        public:
            /**
             * @brief Construct a new Array object with zero element. No allocation is performed
             * 
             * @param allocator the allocator of the array, will not be used
             */
            constexpr explicit Array(const allocator_type& allocator = allocator_type()) noexcept :
                View(), container_(allocator)
            {}

            /**
             * @brief Construct a new Array object, allocate the number of elements defined by shape
             * 
             * @tparam L shape type
             * @tparam IsNotEquivalents<L, ...> check if L is not same than allocator_type, View, Array types
             * @param l shape
             * @param allocator optional allocator
             */
            template< typename L, typename = IsNotEquivalents<L, allocator_type, View, Array> >
            explicit Array(L && l, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), forward<L>(l))
            {}

            /**
             * @brief Construct a new Array object, allocate the number of elements defined by l
             * 
             * @tparam L the scalar type of the Shape
             * @param l the shape
             * @param allocator optional allocator
             */
            template<typename L>
            explicit Array(const initializer_list<L> & l, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), l)
            {}

            /**
             * @brief Construct a new Array object, allocate the number of elements defined by l and init them using data
             * 
             * @tparam L the Shape type
             * @tparam Data The initialization data type
             * @tparam IsNotEquivalent<L, Container> check if L is a Shape
             * @tparam IsNotEquivalents<Data, Array, allocator_type> check if ...
             * @param l shape
             * @param data initial data
             * @param allocator optional allocator
             */
            template<typename L, typename Data,
                typename = IsNotEquivalent<L, Container>,
                typename = IsNotEquivalents<Data, Array, allocator_type>
            >
            explicit Array(L && l, Data && data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), forward<L>(l))
            {
                View::setMem(std::forward<Data>(data));
            }

            /**
             * @brief Construct a new Array object, allocate the number of elements defined by l and init them using data
             * 
             * @tparam L the scalar type of the Shape
             * @tparam Data The initialization data type
             * @tparam IsNotEquivalent<Data, allocator_type> 
             * @param l shape
             * @param data initial data
             * @param allocator optional allocator
             */
            template<typename L, typename Data, typename = IsNotEquivalent<Data, allocator_type>>
            explicit Array(const initializer_list<L> & l, Data && data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), l)
            {
                View::setMem(std::forward<Data>(data));
            }

            /**
             * @brief Construct a new Array object, allocate the number of elements defined by l and init them using data
             * 
             * @tparam L the shape type
             * @tparam Data the scalar type of data
             * @tparam IsNotEquivalent<L, Container> 
             * @param l shape
             * @param data initial data
             * @param allocator optional allocator
             */
            template<typename L, typename Data, typename = IsNotEquivalent<L, Container>>
            explicit Array(L && l, const initializer_list<Data> & data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), forward<L>(l))
            {
                View::setMem(data);
            }

            /**
             * @brief Construct a new Array object, allocate the number of elements defined by l and init them using data
             * 
             * @tparam L the scalar type of the Shape
             * @tparam Data the scalar type of data
             * @param l shape
             * @param data initial data
             * @param allocator optional allocator
             */
            template<typename L, typename Data>
            explicit Array(const initializer_list<L> & l, const initializer_list<Data> & data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), l)
            {
                View::setMem(data);
            }

            /**
             * @brief Construct a new Array object from another view, using the same shape and copy data
             * 
             * @param view the view to take data of
             * @param allocator optional allocator
             */
            explicit Array(const View & view, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(view.shape()), allocator), view.shape())
            {
                View::setMem(view);
            }

            /**
             * @brief Copy construct a new Array object
             * 
             */
            Array(const Array &) = default;

            /**
             * @brief Move construct a new Array object
             * 
             */
            constexpr Array(Array &&) noexcept = default;

            /**
             * @brief Copy assign object
             * 
             * @return Array& the reference to *this
             */
            Array& operator=(const Array &) = default;

            /**
             * @brief Move assign object
             * 
             * @return Array& the reference to *this
             */
            Array& operator=(Array &&) = default;

            /**
             * @brief Get the allocator object const reference
             * 
             * @return allocator_type
             */
            constexpr const allocator_type & get_allocator() const {
                return container_.get_allocator();
            }

            /**
             * @brief Obtain the allocated size independently of the shape
             * 
             * @return constexpr SizeT 
             */
            constexpr SizeT baseSize() const {
                return container_.size();
            }
            
            using View::size;
        };
        /** @} */       // end group View
    }
}

#endif //MA_ARRAY_ARRAY_H