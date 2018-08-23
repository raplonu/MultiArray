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

            pointer data() noexcept { return container_->data(); }
            const_pointer data() const noexcept { return container_->data(); }

            const_pointer begin() const noexcept {return container_->data();}
            const_pointer end() const noexcept {return container_->data() + container_->size();}

            pointer begin() noexcept {return container_->data();}
            pointer end() noexcept {return container_->data() + container_->size();}

            constexpr size_type size() const noexcept { return container_->size(); }
            constexpr allocator_type get_allocator() const { return container_->get_allocator(); }
        };
    }
}

#endif //MA_CONTAINER_SHARED_CONTAINER_H