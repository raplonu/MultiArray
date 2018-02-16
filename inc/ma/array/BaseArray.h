#ifndef MA_ARRAY_BASE_ARRAY_H
#define MA_ARRAY_BASE_ARRAY_H

#include <ma/detail/base.h>

#include <ma/iterator/ShapeIterator.h>
#include <ma/container/SharedContainer.h>

#include <ma/copy/memCopy.h>

namespace ma
{
    namespace array
    {
        template
        <
            typename T, typename Shape,
            typename Alloc,
            typename Container
        >
        class BaseArray;

        template
        <
            typename T, typename Shape,
            typename Alloc = detail::DefaultAlloc<T>,
            typename Container = container::SharedContainer
            <
                T, Alloc, detail::DefaultContainer<T, Alloc>
            >
        >
        class BaseArray : public detail::SuperBase
        {
        public:
            using DataT = T;
            using ShapeT = Shape;
            using AllocT = Alloc;
            using ContainerT = Container;

            using BaseArrayT = BaseArray<DataT, ShapeT, AllocT, ContainerT>;

            using reference = typename ContainerT::reference;
            using const_reference = typename ContainerT::const_reference;

            using pointer = typename ContainerT::pointer;
            using const_pointer = typename ContainerT::const_pointer;

            template<typename IT>
            using GenIt = iterator::ShapeIterator<IT, ShapeT>;

            using const_iterator = GenIt<typename ContainerT::const_iterator>;
            using iterator = GenIt<typename ContainerT::iterator>;
            // using const_iterator = typename iterator::ShapeIterator<typename ContainerT::const_iterator, ShapeT>;
            // using const_reverse_iterator = GenIt<typename ContainerT::const_reverse_iterator>;
            // using reverse_iterator = GenIt<typename ContainerT::reverse_iterator>;

        protected:
            ShapeT shape_;
            ContainerT container_;

        protected:
            explicit BaseArray():
                shape_(0), container_()
            {}

            explicit BaseArray(AllocT const & alloc):
                shape_(0), container_(shape_.size(), alloc)
            {}

            template<typename L>
            explicit BaseArray(L const & size, AllocT const & alloc):
                shape_(size), container_(shape_.size(), alloc)
            {}

            //Size & data copy contructor

            template
            <
                typename L, typename Data,
                typename = typename std::enable_if
                <
                    !detail::has_same_alloc<Data, AllocT>::value
                >::type
            >
            explicit BaseArray(L const & size, Data const & data, AllocT const & alloc):
                shape_(size), container_(shape_.size(), alloc)
            {
                copy::memCopy<DataT>(*this, data);
            }

            template<typename L>
            explicit BaseArray(L const & size, ContainerT container):
                shape_(size), container_(container)
            {}

public:
            BaseArray(BaseArrayT const &) = default;
            BaseArray(BaseArrayT &&) = default;

            BaseArrayT& operator=(BaseArrayT const &) = default;
            BaseArrayT& operator=(BaseArrayT &&) = default;

            ~BaseArray(){}

            template<typename... R>
            BaseArrayT at(R&&... ranges) const
            {
                return BaseArrayT(shape_.subShape(std::forward<R>(ranges)...), container_);
            }

            BaseArrayT operator[](SizeT pos) const
            {
                return BaseArrayT(shape_.subShape(pos), container_);
            }
            operator reference()
            {
                return value(0);
            }

            operator const_reference() const
            {
                return value(0);
            }

            template<typename OData, typename... Args>
            void copyTo(OData && data, Args... args) const
            {
                copy::memCopy<DataT>(data, *this, args...);
            }

            template<typename OData, typename... Args>
            void setMem(OData const & data, Args... args)
            {
                copy::memCopy<DataT>(*this, data, args...);
            }

            ShapeT const & shapeMember() const
            {
                return shape_;
            }

            auto shape() const -> decltype(shape_.shape())
            {
                return shape_.shape();
            }

            SizeT size() const
            {
                return shape_.size();
            }

            SizeT baseSize() const
            {
                return container_.size();
            }

            SizeT step() const
            {
                return shape_.step();
            }

            bool isContigous() const
            {
                return shape_.isContigous();
            }

            SizeT offset() const
            {
                return shape_.at(0);
            }

            pointer ptr()
            {
                return container_.data() + shape_.at(0);
            }

            pointer basePtr()
            {
                return container_.data();
            }

            const_pointer ptr() const
            {
                return container_.data() + shape_.at(0);
            }

            const_pointer basePtr() const
            {
                return container_.data();
            }

            ContainerT & container()
            {
                return container_;
            }

            ContainerT const & container() const
            {
                return container_;
            }

            AllocT get_allocator() const
            {
                return container().get_allocator();
            }

            reference value( SizeT pos = 0)
            {
                return container_.at(shape_.at(pos));
            }

            const_reference value( SizeT pos = 0) const
            {
                return container_.at(shape_.at(pos));
            }

            iterator begin()
            {
                return iterator(container_.begin(), shape_, 0);
            }

            const_iterator begin() const
            {
                return const_iterator(container_.begin(), shape_, 0);
            }

            const_iterator cbegin() const
            {
                return const_iterator(container_.cbegin(), shape_, 0);
            }

            iterator end()
            {
                return iterator(container_.begin(), shape_, shape_.size());
            }

            const_iterator end() const
            {
                return const_iterator(container_.begin(), shape_, shape_.size());
            }

            const_iterator cend() const
            {
                return const_iterator(container_.cbegin(), shape_, shape_.size());
            }

        };

        template
        <
            typename T, typename Range, typename Alloc, typename Container
        >
        class MArray;

        template<typename T, typename Alloc, typename Container>
        class SArray;
    }

    template
    <
    typename T, typename Shape, typename Alloc, typename Container
    >
    void printArray(std::ostream& s, array::BaseArray<T, Shape, Alloc, Container> && a, int margin, int spacing)
    {
        auto shape = a.shape();

        s.put('[');

        if(shape.size() > 1)
        {
            printArray(s, a[0], margin + 1, spacing - 1);
            for (size_t i(1); i < shape[0]; ++i)
            {
                s << std::string(spacing, '\n') << std::string(margin, ' ');
                printArray(s, a[i], margin + 1, spacing - 1);
            }
        }
        else
        {
            s << a[0].value();
            for (size_t i(1); i < shape[0]; ++i)
            {
                s.put(' ');
                s << a[i].value();
            }
        }

        s.put(']');
    }

    template
    <
        typename T, typename Shape, typename Alloc, typename Container
    >
    void printArray(std::ostream& s, array::BaseArray<T, Shape, Alloc, Container> const & a, int margin, int spacing)
    {
        auto shape = a.shape();

        s.put('[');

        if(shape.size() > 1)
        {
            printArray(s, a[0], margin + 1, spacing - 1);
            for (size_t i(1); i < shape[0]; ++i)
            {
                s << std::string(spacing, '\n') << std::string(margin, ' ');
                printArray(s, a[i], margin + 1, spacing - 1);
            }
        }
        else
        {
            s << a[0].value();
            for (size_t i(1); i < shape[0]; ++i)
            {
                s.put(' ');
                s << a[i].value();
            }
        }

        s.put(']');
    }

    // template
    // <
    //     typename T, typename Shape, typename Alloc, typename Container
    // >
    // std::ostream& operator<<(std::ostream& s, array::BaseArray<T, Shape, Alloc, Container> & a)
    // {
    //     if(a.size() > 0)
    //         printArray(s, a, 1, a.shape().size() - 1);
    //     else
    //         s << "[]";
    //     return s;
    // }

    template
    <
        typename T, typename Shape, typename Alloc, typename Container
    >
    std::ostream& operator<<(std::ostream& s, array::BaseArray<T, Shape, Alloc, Container> const & a)
    {
        if(a.size() > 0)
            printArray(s, a, 1, a.shape().size() - 1);
        else
            s << "[]";
        return s;
    }

    template
    <
        typename T, typename Shape, typename Alloc, typename Container
    >
    void printArray(std::ostream& s, array::BaseArray<T, Shape, Alloc, Container> const & a)
    {
        if(a.size() > 0)
            printArray(s, a, 1, a.shape().size() - 1);
        else
            s << "[]";
    }

}

#endif //MA_ARRAY_BASE_ARRAY_H
