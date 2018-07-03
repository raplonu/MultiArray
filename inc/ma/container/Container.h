#ifndef MA_CONTAINER_CONTAINER_H
#define MA_CONTAINER_CONTAINER_H

#include <memory> //allocator_traits

#include <ma/type.h>

namespace ma
{
    namespace container
    {
        template<typename T, typename Allocator = DefaultAlloc<T>>
        class Container {
        public:
            using value_type = T;
            using allocator_type = Allocator;

            using allocator_trait = std::allocator_traits<allocator_type>;


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
            constexpr explicit Container(const Allocator& allocator = Allocator()) noexcept :
                allocator_(allocator), pointer_(), size_(0)
            {}

            constexpr explicit Container(size_type size, const Allocator& allocator = Allocator()):
                allocator_(allocator), pointer_(allocator_.allocate(size)), size_(size)
            {}

            constexpr Container(const Container&) = default;
            constexpr Container(Container&&) noexcept = default;

            Container & operator=(const Container&) = default;
            Container & operator=(Container&&) noexcept = default;

            ~Container()
            {
                allocator_.deallocate(pointer_, size_);
            }

            pointer data() noexcept { return pointer_; }
            const_pointer data() const noexcept { return pointer_; }

            constexpr size_type size() const noexcept { return size_; }
            constexpr allocator_type get_allocator() const { return allocator_; }
        };
    }
}

#endif //MA_CONTAINER_CONTAINER_H