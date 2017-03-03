#ifndef CONTAINER_H
#define CONTAINER_H

#include <memory>

namespace ma
{
    namespace data
    {
        template<typename T, typename Allocator>
        class Container
        {
            Allocator allocator_;
            SizeT size_;
            T* ptr_;

        public:
            explicit Container(Allocator const & allocator = Allocator()):
                allocator_(allocator), size_(0), ptr_(nullptr)
            {}

            explicit Container(SizeT size, Allocator const & allocator = Allocator()):
                allocator_(allocator),
                size_(size),
                ptr_(allocator_.allocate(size_))
            {}

            template<typename... Args>
            Container(SizeT size, Args... args):
                allocator_(),
                size_(size),
                ptr_(allocator_.allocate(size_, args...))
            {}

            template<typename... Args>
            Container(SizeT size, Allocator const & allocator, Args... args):
                allocator_(allocator),
                size_(size),
                ptr_(allocator_.allocate(size_))
            {}

            ~Container()
            {
                if(allocated())
                    allocator_.deallocate(ptr_, size_);
            }

            T const * ptr() const
            {
                return ptr_;
            }

            T * ptr()
            {
                return ptr_;
            }

            operator T const *() const
            {
                return ptr();
            }
            
            operator T *()
            {
                return ptr();
            }

            SizeT length() const
            {
                return size_ * sizeof(T);
            }

            SizeT size() const
            {
                return size_;
            }

            bool allocated() const
            {
                return ptr_ != nullptr;
            }
        };

        template<typename T, typename Allocator, typename... Args>
        std::shared_ptr<Container<T, Allocator>> makeSharedContainer(Args... args)
        {
            return std::make_shared<Container<T, Allocator>>(args...);
        }
    }

}

#endif //CONTAINER_H
