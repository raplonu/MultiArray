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
#ifndef MA_ARRAY_ARRAY_VIEW
#define MA_ARRAY_ARRAY_VIEW


#include <ma_api/type.h>
#include <ma_api/traits.h>
#include <ma_api/function.h>
#include <ma_api/printData.h>

#include <ma_api/iterator/ShapeIterator.h>
#include <ma_api/algorithm/copy.h>

namespace ma
{
    namespace array
    {
        /**
         * @brief View of an array through a generic shape
         * 
         * @tparam T the underlying array type
         * @tparam Allocator Need to specify what kind of allocator produce the array,
         * must be change to the pointer implementation type
         * @tparam Shape The shape type
         */
        template <typename T, typename Allocator, typename Shape>
        class ArrayView
        {
        public:
            using value_type = T;
            using allocator_type = Allocator;

            using allocator_trait = ma::allocator_traits<allocator_type>;

            using reference         = typename allocator_trait::reference;
            using const_reference         = typename allocator_trait::const_reference;

            using pointer            = typename allocator_trait::pointer;
            using const_pointer      = typename allocator_trait::const_pointer;
            using void_pointer       = typename allocator_trait::void_pointer;
            using const_void_pointer = typename allocator_trait::const_void_pointer;
            using difference_type    = typename allocator_trait::difference_type;
            using size_type          = typename allocator_trait::size_type;
            
            using propagate_on_container_copy_assignment = typename allocator_trait::propagate_on_container_copy_assignment;
            using propagate_on_container_move_assignment = typename allocator_trait::propagate_on_container_move_assignment;
            using propagate_on_container_swap            = typename allocator_trait::propagate_on_container_swap;

            using const_iterator = iterator::ShapeIterator<const_pointer, Shape>;
            using iterator = iterator::ShapeIterator<pointer, Shape>;

        protected:
            Shape shape_;
            pointer ptr_;

        public:
            /**
             * @brief Default constructor. Constructs an empty container.
             * 
             */
            constexpr ArrayView() noexcept :
                shape_(), ptr_()
            {}

            /**
             * @brief Constructs the container with count copies of elements with value value.
             * 
             * @tparam L 
             * @param size 
             * @param ptr 
             */
            template<typename L>
            explicit ArrayView(L && size, pointer ptr) noexcept :
                shape_(forward<L>(size)), ptr_(ptr)
            {}

            template<typename Size>
            explicit ArrayView(const std::initializer_list<Size> & size, pointer ptr) noexcept :
                shape_(size), ptr_(ptr)
            {}

            ArrayView(const ArrayView &) = default;
            constexpr ArrayView(ArrayView &&) noexcept = default;
            
            ArrayView& operator=(const ArrayView &) = default;
            ArrayView& operator=(ArrayView &&) noexcept = default;

        protected:
            constexpr explicit ArrayView(Shape && shape, pointer ptr) noexcept :
                shape_(move(shape)), ptr_(ptr)
            {}

        public:
            ~ArrayView() = default;

            template<typename... R>
            ArrayView at(R&&...ranges) const {
                return ArrayView(shape_.subShape(forward<R>(ranges)...), ptr_);
            }

            /**
             * @brief Return a view on the data with the inner dimension closed at pos
             * 
             * @param pos the position where to close the inner dimension
             * @return ArrayView 
             */
            ArrayView operator[](SizeT pos) const {
                return ArrayView(shape_.closeAt(pos), ptr_);
            }

            /**
             * @brief access to the first element iterator
             * 
             * @return iterator 
             */
            iterator begin() {
                return iterator(ptr_, shape_, 0);
            }

            /**
             * @brief access to the first element const iterator
             * 
             * @return const_iterator 
             */
            const_iterator begin() const {
                return const_iterator(ptr_, shape_, 0);
            }

            /**
             * @brief access to the last element iterator
             * 
             * @return iterator 
             */
            iterator end() {
                return iterator(ptr_, shape_, size());
            }

            /**
             * @brief access to the last element const iterator
             * 
             * @return const_iterator 
             */
            const_iterator end() const {
                return const_iterator(ptr_, shape_, size());
            }

            /**
             * @brief access the a specific element that has the pos distance from the beginning of the shape
             * 
             * @param pos 
             * @return reference 
             */
            reference val(SizeT pos = 0) {
                return reference(ptr_[shape_.at(pos)]);
            }

            /**
             * @brief access the a specific element that has the pos distance from the beginning of the shape
             * 
             * @param pos 
             * @return const_reference 
             */
            constexpr const_reference val(SizeT pos = 0) const {
                return const_reference(*(ptr_ + shape_.at(pos)));
            }

            /**
             * @brief return the pointer to the start of the array
             * 
             * @return pointer 
             */
            pointer ptr() noexcept {
                return ptr_ + shape_.at(0);
            }

            /**
             * @brief return the const pointer to the start of the array
             * 
             * @return const_pointer 
             */
            constexpr const_pointer ptr() const noexcept {
                return ptr_ + shape_.at(0);
            }

            constexpr SizeT size() const noexcept {
                return shape_.size();
            }

            constexpr bool empty() const noexcept {
                return size() == 0;
            }

            constexpr SizeT step() const noexcept {
                return shape_.step();
            }

            VectRange shape() const {
                return shape_.shape();
            }

            constexpr SizeT ndim() const {
                return shape_.ndim();
            }

            constexpr bool contiguous() const noexcept {
                return shape_.contiguous();
            }

            template<typename OData, typename... Args>
            void copyTo(OData && data, Args && ... args) const {
                algorithm::multiCopy<T>(forward<OData>(data), *this, forward<Args>(args)...);
            }

            template<typename OData, typename... Args>
            void setMem(const OData & data, Args && ... args) {
                algorithm::multiCopy<T>(*this, data, forward<Args>(args)...);
            }

            template<typename TT, typename... Args>
            void setMem(const std::initializer_list<TT> & data, Args && ... args) {
                algorithm::multiCopy<T>(*this, data, forward<Args>(args)...);
            }
        };

        template <typename T, typename Allocator, typename Shape>
        std::ostream& operator<<(std::ostream& s, const ArrayView<T, Allocator, Shape> & view) {
            printData(s, view);
            return s;
        }

    }
}

#endif //MA_ARRAY_ARRAY_VIEW