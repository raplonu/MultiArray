#ifndef MA_ARRAY_M_ARRAY_H
#define MA_ARRAY_M_ARRAY_H

#include <ma/detail/base.h>

#include <ma/shape/MShape.h>

#include <ma/array/BaseArray.h>

namespace ma
{
    namespace array
    {
        template
        <
            typename T, typename Range = range::LinearRange,
            typename Alloc = detail::DefaultAlloc<T>,
            typename Container = container::SharedContainer
            <
                T, Alloc, detail::DefaultContainer<T, Alloc>
            >
        >
        class MArray : public BaseArray<T, shape::MShape<Range>, Alloc, Container>
        {
        public:
            using DataT = T;
            using RangeT = Range;
            using ShapeT = shape::MShape<RangeT>;
            using AllocT = Alloc;
            using ContainerT = Container;

            using MArrayT = MArray<DataT, RangeT, AllocT, ContainerT>;
            using BaseT = BaseArray<DataT, ShapeT, AllocT, ContainerT>;

            template<typename OAlloc>
            using OBaseArrayAllocT = BaseArray<DataT, ShapeT, OAlloc, ContainerT>;

            template<typename OShape>
            using OBaseArrayT = BaseArray<DataT, OShape, AllocT, ContainerT>;


            template<typename IT>
            using GenIt = iterator::ShapeIterator<IT, ShapeT>;

            using const_iterator = GenIt<typename ContainerT::const_iterator>;
            using iterator = GenIt<typename ContainerT::iterator>;
            // using const_iterator = typename iterator::ShapeIterator<typename ContainerT::const_iterator, ShapeT>;


            //Void construtor

            explicit MArray(AllocT const & alloc = AllocT()):
                BaseT(alloc)
            {}

            explicit MArray(SizeT size, AllocT const & alloc = AllocT()):
                BaseT(size, alloc)
            {}

            //template detect everithing like vector
            explicit MArray(std::vector<SizeT> const & size, AllocT const & alloc = AllocT()):
                BaseT(size, alloc)
            {}

            explicit MArray(std::initializer_list<SizeT> const & size, AllocT const & alloc = AllocT()):
                BaseT(size, alloc)
            {}


            //Size and data construtor
            template<typename Data, typename = typename std::enable_if<!std::is_base_of<detail::SuperBase, Data>::value>::type>
            explicit MArray(SizeT size, Data const & data, AllocT const & alloc = AllocT()):
                BaseT(size, data, alloc)
            {}

            //template detect everithing like vector
            template<typename Data, typename = typename std::enable_if<!std::is_base_of<detail::SuperBase, Data>::value>::type>
            explicit MArray(std::vector<SizeT> const & size, Data const & data, AllocT const & alloc = AllocT()):
                BaseT(size, data, alloc)
            {}

            template<typename Data, typename = typename std::enable_if<!std::is_base_of<detail::SuperBase, Data>::value>::type>
            explicit MArray(std::initializer_list<SizeT> const & size, Data const & data, AllocT const & alloc = AllocT()):
                BaseT(size, data, alloc)
            {}

            //Size and initializer_list data construtor

            explicit MArray(SizeT size, std::initializer_list<DataT> const & data, AllocT const & alloc = AllocT()):
                BaseT(size, data, alloc)
            {}

            //template detect everithing like vector
            explicit MArray(std::vector<SizeT> const & size, std::initializer_list<DataT> const & data, AllocT const & alloc = AllocT()):
                BaseT(size, data, alloc)
            {}

            explicit MArray(std::initializer_list<SizeT> const & size, std::initializer_list<DataT> const & data, AllocT const & alloc = AllocT()):
                BaseT(size, data, alloc)
            {}


            //Sized data constuctor

            template
            <
                typename Data,
                typename = typename std::enable_if
                <
                    !std::is_base_of<detail::SuperBase, Data>::value &&
                    detail::has_size<Data>::value
                >::type
            >
            explicit MArray(Data const & data, AllocT const & alloc = AllocT()):
                BaseT(size(data), data, alloc)
            {}

            // explicit MArray(std::initializer_list<DataT> const & data, AllocT const & alloc = AllocT()):
            //     BaseT(size(data), data, alloc)
            // {}

            template
            <
                typename Data,
                typename = typename std::enable_if
                <
                    std::is_base_of<detail::SuperBase, Data>::value// &&
                    // !detail::has_same_alloc<Data, AllocT>::value
                >::type,
                typename = typename std::enable_if
                <
                    // std::is_base_of<detail::SuperBase, Data>::value &&
                    !detail::has_same_alloc<Data, AllocT>::value
                >::type
            >
            explicit MArray(Data const & data, AllocT const & alloc = AllocT()):
                BaseT(data.shape(), data, alloc)
            {}

            //shape && same size data

            template<typename OShape>
            explicit MArray(OBaseArrayT<OShape> & data, AllocT const & alloc = AllocT())
            {
                BaseT::shape_ = ShapeT(data.shapeMember());

                if( data.container().get_allocator() == alloc )
                {
                    BaseT::container_ = data.container();
                }
                else
                {
                    BaseT::container_ = ContainerT(BaseT::size(), alloc);
                    copy::memCopy<DataT>(*this, data);
                }
            }

            // template<typename OAlloc>
            // explicit MArray(OBaseArrayAllocT<OAlloc> const & data, AllocT const & alloc = AllocT()):
            //     BaseT(data.shape(), data, alloc)
            // {}

            template<typename L>
            explicit MArray(L const & size, OBaseArrayT<shape::SimpleShape> const & data, AllocT const & alloc = AllocT())
            {
                BaseT::shape_ = ShapeT(size, data.offset());

                if(BaseT::size() != data.size())
                    throw std::length_error( "Error shape don't compute" );

                if( data.container().get_allocator() == alloc )
                {
                    BaseT::container_ = data.container();
                }
                else
                {
                    BaseT::container_ = ContainerT(BaseT::size(), alloc);
                    copy::memCopy<DataT>(*this, data);
                }
            }

            explicit MArray(std::initializer_list<SizeT> const & size, OBaseArrayT<shape::SimpleShape> const & data, AllocT const & alloc = AllocT())
            {
                BaseT::shape_ = ShapeT(size, data.offset());

                if(BaseT::size() != data.size())
                    throw std::length_error( "Error shape don't compute" );

                if( data.container().get_allocator() == alloc )
                {
                    BaseT::container_ = data.container();
                }
                else
                {
                    BaseT::container_ = ContainerT(BaseT::size(), alloc);
                    copy::memCopy<DataT>(*this, data);
                }
            }

            MArray(ShapeT && size, ContainerT container):
                BaseT(std::move(size), container)
            {}

            MArray(MArrayT const &) = default;
            MArray(MArrayT &&) = default;

            MArrayT& operator=(MArrayT const &) = default;
            MArrayT& operator=(MArrayT &&) = default;

            ~MArray(){}

            MArray operator=(DataT val)
            {
                BaseT::value(0) = val;
                return *this;
            }

            template<typename... R>
            MArrayT at(R&&... ranges) const
            {
                return MArrayT(BaseT::shape_.subShape(std::forward<R>(ranges)...), BaseT::container_);
            }

            MArrayT operator[](SizeT pos) const
            {
                return MArrayT(BaseT::shape_.subShape(pos), BaseT::container_);
            }
        };


    }

    template
    <
        typename T, typename Range = range::LinearRange,
        typename Alloc = detail::DefaultAlloc<T>,
        typename Container = container::SharedContainer
        <
            T, Alloc, detail::DefaultContainer<T, Alloc>
        >
    >
    using MArray = array::MArray<T, Range, Alloc, Container>;
}

#endif //MA_ARRAY_M_ARRAY_H
