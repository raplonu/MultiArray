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
#ifndef MA_CONTAINER_SHARED_CONTAINER_H
#define MA_CONTAINER_SHARED_CONTAINER_H

#include <memory> // shared_ptr & make_shared

#include <ma_api/container/Container.h>

namespace ma
{
    namespace container
    {
        template<typename T, typename Allocator = DefaultAlloc<T>, typename Container = Container<T, Allocator>>
        class SharedContainer {
        public:
            using value_type     = T;
            using allocator_type = Allocator;
            using container_type = Container;

            using allocator_trait = ma::allocator_traits<allocator_type>;

            using pointer            = typename allocator_trait::pointer;
            using const_pointer      = typename allocator_trait::const_pointer;
            using void_pointer       = typename allocator_trait::void_pointer;
            using const_void_pointer = typename allocator_trait::const_void_pointer;
            using difference_type    = typename allocator_trait::difference_type;
            using size_type          = typename allocator_trait::size_type;
            
            using propagate_on_container_copy_assignment = typename allocator_trait::propagate_on_container_copy_assignment;
            using propagate_on_container_move_assignment = typename allocator_trait::propagate_on_container_move_assignment;
            using propagate_on_container_swap            = typename allocator_trait::propagate_on_container_swap;

            using shared_container = std::shared_ptr<container_type>;

        protected:
            shared_container container_;

        public:
            constexpr explicit SharedContainer(const Allocator& allocator = Allocator()) noexcept :
                container_(std::make_shared<container_type>(allocator))
            {}

            constexpr explicit SharedContainer(size_type size, const Allocator& allocator = Allocator()):
                container_(std::make_shared<container_type>(size, allocator))
            {}

            constexpr SharedContainer(const SharedContainer&) noexcept = default;
            constexpr SharedContainer(SharedContainer&&) noexcept = default;

            SharedContainer & operator=(const SharedContainer&) noexcept = default;
            SharedContainer & operator=(SharedContainer&&) noexcept = default;

            ~SharedContainer() = default;

            pointer ptr() noexcept { return container_->ptr(); }
            const_pointer ptr() const noexcept { return container_->ptr(); }

            const_pointer begin() const noexcept {return container_->ptr();}
            const_pointer end() const noexcept {return container_->ptr() + container_->size();}

            pointer begin() noexcept {return container_->ptr();}
            pointer end() noexcept {return container_->ptr() + container_->size();}

            constexpr size_type size() const noexcept { return container_->size(); }
            constexpr allocator_type get_allocator() const { return container_->get_allocator(); }
        };
    }
}

#endif //MA_CONTAINER_SHARED_CONTAINER_H