#ifndef MA_ARRAY_S_ARRAY_H
#define MA_ARRAY_S_ARRAY_H

#include <ma/detail/base.h>

#include <ma/shape/SimpleShape.h>

#include <ma/array/BaseArray.h>
// #include <ma/array/MArray.h>

namespace ma
{
    namespace array
    {

        template
        <
            typename T, typename Alloc = detail::DefaultAlloc<T>,
            typename Container = container::SharedContainer
            <
                T, Alloc, detail::DefaultContainer<T, Alloc>
            >
        >
        class SArray : public BaseArray<T, shape::SimpleShape, Alloc, Container>
        {
        public:
            using DataT = T;
            using ShapeT = shape::SimpleShape;
            using AllocT = Alloc;
            using ContainerT = Container;

            using SArrayT = SArray<DataT, AllocT, ContainerT>;
            using BaseT = BaseArray<DataT, ShapeT, AllocT, ContainerT>;

            template
            <
                typename OAlloc,
                typename = typename std::enable_if
                <
                    !std::is_same<AllocT, OAlloc>::value
                >::type
            >
            using OSArrayT = SArray<DataT, OAlloc, ContainerT>;

            template
            <
                typename OShape, typename OAlloc,
                typename = typename std::enable_if
                <
                    !std::is_same<AllocT, OAlloc>::value
                >::type
            >
            using OMArrayT = MArray<DataT, OShape, OAlloc, ContainerT>;

            template<typename IT>
            using GenIt = iterator::ShapeIterator<IT, ShapeT>;

            using const_iterator = GenIt<typename ContainerT::const_iterator>;
            using iterator = GenIt<typename ContainerT::iterator>;
            // using const_iterator = typename iterator::ShapeIterator<typename ContainerT::const_iterator, ShapeT>;

            // using BaseArray<T, shape::SimpleShape, Alloc, Container>::BaseArray;

            //Void constructor

            explicit SArray(AllocT const & alloc = AllocT()):
                BaseT(alloc)
            {}

            //Size constructor

            template
            <
                typename Size,
                typename = typename std::enable_if<std::is_integral<Size>::value>::type
            >
            explicit SArray(Size size, AllocT const & alloc = AllocT()):
                BaseT(size, alloc)
            {}

            //Data constructor

            template
            <
                typename Size, typename Data,
                typename = typename std::enable_if
                <
                    std::is_integral<Size>::value &&
                    !detail::has_same_alloc<Data, AllocT>::value
                >::type
            >
            explicit SArray(Size size, Data const & data, AllocT const & alloc = AllocT()):
                BaseT(size, data, alloc)
            {}

            template
            <
                typename Size,
                typename = typename std::enable_if<std::is_integral<Size>::value>::type
            >
            explicit SArray(Size size, std::initializer_list<DataT> const & data, AllocT const & alloc = AllocT()):
                BaseT(size, data, alloc)
            {}

            //Sized data constructor

            template
            <
                typename Data,
                typename = typename std::enable_if
                <
                    detail::has_size<Data>::value &&
                    !detail::has_same_alloc<Data, AllocT>::value
                >::type
            >
            explicit SArray(Data const & data, AllocT const & alloc = AllocT()):
                BaseT(size(data), data, alloc)
            {}

            template<typename OShape>
            explicit SArray(BaseArray<DataT, OShape, AllocT, ContainerT> & data, AllocT const & alloc = AllocT())
            {
                BaseT::shape_ = ShapeT(data.offset(), data.size());
                if( data.isContigous() && (data.container().get_allocator() == alloc) )
                {
                    BaseT::container_ = data.container();
                }
                else
                {
                    BaseT::container_ = ContainerT(BaseT::size(), alloc);
                    copy::memCopy<DataT>(*this, data);
                }
            }

            SArray(ShapeT && shape, ContainerT & container):
                BaseT(shape, container)
            {}

            SArray(SArrayT const & oba) = default;
            SArray(SArrayT && oba) = default;

            SArrayT& operator=(SArrayT const & oba) = default;
            SArrayT& operator=(SArrayT && oba) = default;

            ~SArray(){}

            SArrayT operator=(DataT val)
            {
                BaseT::value(0) = val;
                return *this;
            }


            template<typename... R>
            SArrayT at(R&&... ranges)
            {
                return SArrayT(BaseT::shape_.subShape(std::forward<R>(ranges)...), BaseT::container_);
            }

            SArrayT operator[](SizeT pos)
            {
                return SArrayT(BaseT::shape_.subShape(pos), BaseT::container_);
            }


        };


    }

    template
    <
        typename T, typename Alloc = detail::DefaultAlloc<T>,
        typename Container = container::SharedContainer
        <
            T, Alloc, detail::DefaultContainer<T, Alloc>
        >
    >
    using SArray = array::SArray<T, Alloc, Container>;
}

#endif //MA_ARRAY_S_ARRAY_H
