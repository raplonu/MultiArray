#ifndef MA_POINTER_H
#define MA_POINTER_H

#include <initializer_list>

#include "data/Container.h"
#include "data/containerFunction.h"

namespace ma
{
    template<typename T>
    class SuperBase{};

    template<typename T, typename Allocator>
    class BasePointer : public SuperBase<T>
    {
    public:
        using DataT = T;
        using AllocatorT = Allocator;
        using allocator_type = Allocator;

        using BasePointerT = BasePointer<DataT, AllocatorT>;

        using Container = data::Container<DataT, AllocatorT>;
        using ContainerPtr = std::shared_ptr<Container>;

    protected:
        ContainerPtr container_;

    public:
        explicit BasePointer(Allocator const & allocator = Allocator()):
            container_(data::makeSharedContainer<DataT, AllocatorT>(allocator))
        {}

        // explicit BasePointer(SizeT size, Allocator const & allocator = Allocator()):
        //     container_(data::makeSharedContainer<DataT, AllocatorT>(size, allocator))
        // {}

        template<typename... Args>
        BasePointer(SizeT count, Args... args):
            container_(data::makeSharedContainer<DataT, AllocatorT>(count, args...))
        {}

        template<typename... Args>
        BasePointer(SizeT count, Allocator const & allocator = Allocator(), Args... args):
            container_(data::makeSharedContainer<DataT, AllocatorT>(count, allocator, args...))
        {}

        template<typename Data, typename... Args>
        BasePointer(SizeT count, Data const & data, Args... args):
            container_(data::makeSharedContainer<DataT, AllocatorT>(count, args...))
        {
            container::setMem<
                AllocatorT,
                typename get_allocator_type<Data, DefaultAlloc<DataT>>::type
            >(ptr(), common::convert<DataT>(data), std::min(size(), common::size(data)));
        }

        template<typename... Args>
        BasePointer(SizeT count, std::initializer_list<DataT> const & data, Args... args):
            container_(data::makeSharedContainer<DataT, AllocatorT>(count, args...))
        {
            container::setMem<
                AllocatorT,
                DefaultAlloc<DataT>
            >(ptr(), common::convert<DataT>(data), std::min(size(), common::size(data)));
        }

        template<typename Data, typename... Args>
        BasePointer(SizeT count, Data const & data, Allocator const & allocator = Allocator(), Args... args):
            container_(data::makeSharedContainer<DataT, AllocatorT>(count, allocator, args...))
        {
            container::setMem<
                AllocatorT,
                typename get_allocator_type<Data, DefaultAlloc<DataT>>::type
            >(ptr(), common::convert<DataT>(data), std::min(size(), common::size(data)));
        }

        template<typename... Args>
        BasePointer(SizeT count, std::initializer_list<DataT> const & data, Allocator const & allocator = Allocator(), Args... args):
            container_(data::makeSharedContainer<DataT, AllocatorT>(count, allocator, args...))
        {
            container::setMem<
                AllocatorT,
                DefaultAlloc<DataT>
            >(ptr(), common::convert<DataT>(data), std::min(size(), common::size(data)));
        }

        template<typename OtherPointer, typename = typename std::enable_if<std::is_base_of<SuperBase<DataT>, OtherPointer>::value>::type, typename... Args>
        BasePointer(OtherPointer const & data, Args... args):
            container_(data::makeSharedContainer<DataT, AllocatorT>(data.size(), args...))
        {
            container::setMem<
                AllocatorT,
                OtherPointer::allocator_type
            >(ptr(), common::convert<DataT>(data), size());
        }

        template<typename OtherPointer, typename = typename std::enable_if<std::is_base_of<SuperBase<DataT>, OtherPointer>::value>::type, typename... Args>
        BasePointer(OtherPointer const & data, Allocator const & allocator = Allocator(), Args... args):
            container_(data::makeSharedContainer<DataT, AllocatorT>(data.size(), allocator, args...))
        {
            container::setMem<
                AllocatorT,
                OtherPointer::allocator_type
            >(ptr(), common::convert<DataT>(data), size());
        }


        BasePointer(BasePointerT const & bp) = default;
        BasePointerT& operator=(BasePointerT const & bp) = default;


        DataT const * ptr() const
        {
            return container_->ptr();
        }

        DataT * ptr()
        {
            return container_->ptr();
        }

        operator DataT const *() const
        {
            return ptr();
        }

        operator DataT *()
        {
            return ptr();
        }

        template<typename... Args>
        DataT valueAt(SizeT pos, Args... args) const
        {
            DataT data;
            container::setMem<
                DefaultAlloc<DataT>,
                AllocatorT
            >(&data, ptr() + pos, 1ul, args...);
            return data;
        }

        template<typename... Args>
        DataT value(Args... args) const
        {
            DataT data;
            container::setMem<
                DefaultAlloc<DataT>,
                AllocatorT
            >(&data, ptr(), 1ul, args...);
            return data;
        }

        operator DataT() const
        {
            return value();
        }

        SizeT size() const
        {
            return container_->size();
        }

        SizeT length() const
        {
            return container_->length();
        }

        bool allocated() const
	    {
		return container_->allocated();
	    }
    };

    template<typename T>
    using Pointer = BasePointer<T, DefaultAlloc<T>>;
}

#endif //MA_POINTER_H
