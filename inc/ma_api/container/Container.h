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
#ifndef MA_CONTAINER_CONTAINER_H
#define MA_CONTAINER_CONTAINER_H

#include <iostream> //allocator_traits
#include <memory> //allocator_traits

#include <ma_api/type.h>
#include <ma_api/traits.h>
#include <ma_api/function.h>

namespace ma
{
    namespace container
    {
        /**
         * @brief Define a Container structure that old a exclusive contiguous buffer
         * 
         * @tparam T the underlying type
         * @tparam DefaultAlloc<T> the allocator type
         */
        template<typename T, typename Allocator = DefaultAlloc<T>>
        class Container {
        public:
            using value_type = T;
            using allocator_type = Allocator;

            using allocator_trait = ma::allocator_traits<allocator_type>;



            using reference          = typename allocator_trait::reference;
            using const_reference    = typename allocator_trait::const_reference;
            using pointer            = typename allocator_trait::pointer;
            using const_pointer      = typename allocator_trait::const_pointer;
            using void_pointer       = typename allocator_trait::void_pointer;
            using const_void_pointer = typename allocator_trait::const_void_pointer;
            using difference_type    = typename allocator_trait::difference_type;
            using size_type          = typename allocator_trait::size_type;
            
            using propagate_on_container_copy_assignment = typename allocator_trait::propagate_on_container_copy_assignment;
            using propagate_on_container_move_assignment = typename allocator_trait::propagate_on_container_move_assignment;
            using propagate_on_container_swap            = typename allocator_trait::propagate_on_container_swap;

        protected:
            allocator_type allocator_;
            pointer pointer_;
            size_type size_;

        public:
            constexpr explicit Container(const allocator_type& allocator = Allocator()) noexcept :
                allocator_(allocator), pointer_(nullptr), size_(0)
            {}

            constexpr explicit Container(size_type size, const allocator_type& allocator = Allocator()):
                allocator_(allocator), pointer_(allocator_.allocate(size)), size_(size)
            {}

            constexpr Container(const Container & oc) :
                allocator_(oc.allocator_), pointer_(allocator_.allocate(oc.size_)), size_(oc.size_)
            {}

            constexpr Container(Container && oc) noexcept :
                allocator_(std::move(oc.allocator_)), pointer_(exchange(oc.pointer_, pointer((value_type*)nullptr))), size_(oc.size_)
            {}

            Container & operator=(const Container & oc) {
                reset();

                allocator_ = oc.allocator_;
                size_ = oc.size_;

                pointer_ = allocator_.allocate(size_);

                return *this;
            }

            Container & operator=(Container && oc) noexcept {
                allocator_ = std::move(oc.allocator_);
                size_ = oc.size_;

                pointer_ = exchange(oc.pointer_, pointer(nullptr));

                return *this;
            }

            ~Container() {
                reset();
            }

            void reset() {
                if(ptrValid(pointer_)) allocator_.deallocate(pointer_, size_);
            }

            pointer ptr() noexcept { return pointer_; }
            const_pointer ptr() const noexcept { return pointer_; }

            const_pointer begin() const noexcept {return pointer_;}
            const_pointer end() const noexcept {return pointer_ + size_;}

            pointer begin() noexcept {return pointer_;}
            pointer end() noexcept {return pointer_ + size_;}

            constexpr size_type size() const noexcept { return size_; }
            constexpr const allocator_type & get_allocator() const { return allocator_; }
        };
    }
}

#endif //MA_CONTAINER_CONTAINER_H