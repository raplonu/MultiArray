#ifndef MA_CONTAINER_SHARED_CONTAINER_H
#define MA_CONTAINER_SHARED_CONTAINER_H

#include <memory>
#include <ma/detail/base.h>

namespace ma
{
    namespace container
    {
        template
        <
            typename T,
            typename Alloc = detail::DefaultAlloc<T>,
            typename Container = detail::DefaultContainer<T, Alloc>
        >
        class SharedContainer
        {
            // Concept requirements.
            using DataT = T;
            using AllocT = Alloc;
            using ContainerT = Container;
            using ContainerSharedT = std::shared_ptr<ContainerT>;

            using SharedContainerT = SharedContainer<DataT, AllocT, ContainerT>;
            //using BaseT = SuperBaseData<DataT>;

        public:
            using value_type = typename ContainerT::value_type;
            using pointer = typename ContainerT::pointer;
            using const_pointer = typename ContainerT::const_pointer;
            using reference = typename ContainerT::reference;
            using const_reference = typename ContainerT::const_reference;
            using iterator = typename ContainerT::iterator;
            using const_iterator = typename ContainerT::const_iterator;
            using const_reverse_iterator = typename ContainerT::const_reverse_iterator;
            using reverse_iterator = typename ContainerT::reverse_iterator;
            using size_type = typename ContainerT::size_type;
            using difference_type = typename ContainerT::difference_type;
            using allocator_type = typename ContainerT::allocator_type;

        protected:
            ContainerSharedT container_;

        public:
            SharedContainer():SharedContainer(AllocT()){}

            explicit SharedContainer(AllocT const & alloc):
                container_(std::make_shared<ContainerT>(alloc))
            {}

            explicit SharedContainer(size_type n, AllocT const & alloc = AllocT()):
                container_(std::make_shared<ContainerT>(n, alloc))
            {}

            explicit SharedContainer
            (
                size_type n, value_type const & value,
                AllocT const & alloc = AllocT()
            ):
                container_(std::make_shared<ContainerT>(n, value, alloc))
            {}

            template<typename InputIt>
            SharedContainer
            (
                InputIt first, InputIt last,
                AllocT const & alloc = AllocT()
            ):
                container_(std::make_shared<ContainerT>(first, last, alloc))
            {}

            SharedContainer
            (
                std::initializer_list<T> init,
                AllocT const & alloc = AllocT()
            ):
                container_(std::make_shared<ContainerT>(init.begin(), init.end(), alloc))
            {}

            SharedContainer(SharedContainerT const & bd) = default;
            SharedContainer(SharedContainerT && bd) = default;

            // SharedContainer(SharedContainerT const & bd,
            // AllocT const & alloc = AllocT()):container_(bd.container_, alloc){}
            // SharedContainer(SharedContainerT && bd,
            // AllocT const & alloc = AllocT()):container_(std::move(bd.container_, alloc)){}

            SharedContainerT& operator=(SharedContainerT const & bd)
            {
                container_ = bd.container_;
                return *this;
            }

            SharedContainerT& operator=(SharedContainerT && bd)
            {
                container_ = std::move(bd.container_);
                return *this;
            }

            ~SharedContainer(){}

            allocator_type get_allocator() const
            {
                return container_->get_allocator();
            }

            reference at( size_type pos )
            {
                return container_->operator[](pos);
            }

            const_reference at( size_type pos ) const
            {
                return container_->operator[](pos);
            }

            reference operator[]( size_type pos )
            {
                return container_->operator[](pos);
            }

            const_reference operator[]( size_type pos ) const
            {
                return container_->operator[](pos);
            }

            reference front()
            {
                return container_->front();
            }

            const_reference front() const
            {
                return container_->front();
            }

            reference back()
            {
                return container_->back();
            }

            const_reference back() const
            {
                return container_->back();
            }

            pointer data() noexcept
            {
                return container_->data();
            }

            const_pointer data() const
            {
                return container_->data();
            }

            iterator begin()
            {
                return container_->begin();
            }

            const_iterator begin() const
            {
                return container_->begin();
            }

            const_iterator cbegin() const
            {
                return container_->cbegin();
            }

            iterator rbegin()
            {
                return container_->rbegin();
            }

            const_iterator rbegin() const
            {
                return container_->rbegin();
            }

            const_iterator crbegin() const
            {
                return container_->crbegin();
            }

            iterator end()
            {
                return container_->end();
            }

            const_iterator end() const
            {
                return container_->end();
            }

            const_iterator cend() const
            {
                return container_->cend();
            }

            iterator rend()
            {
                return container_->rend();
            }

            const_iterator rend() const
            {
                return container_->rend();
            }

            const_iterator crend() const
            {
                return container_->crend();
            }

            bool empty() const
            {
                return container_->empty();
            }

            size_type size() const
            {
                return container_->size();
            }

            size_type max_size() const
            {
                return container_->max_size();
            }

            void reserve( size_type new_cap )
            {
                container_->reserve(new_cap);
            }

            size_type capacity() const
            {
                return container_->capacity();
            }

            void shrink_to_fit()
            {
                container_->shrink_to_fit();
            }

            void clear()
            {
                container_->clear();
            }

            iterator insert( iterator pos, const T& value )
            {
                return container_->insert(pos, value);
            }

            iterator insert( const_iterator pos, const T& value )
            {
                return container_->insert(pos, value);
            }

            iterator insert( const_iterator pos, T&& value )
            {
                return container_->insert(pos, std::move(value));
            }

            void insert( iterator pos, size_type count, const T& value )
            {
                container_->insert(pos, count, value);
            }

            //TODO finish add method


        };
    }
}

#endif //MA_CONTAINER_SHARED_CONTAINER_H
