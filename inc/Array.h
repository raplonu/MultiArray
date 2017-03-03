#ifndef ARRAY_H
#define ARRAY_H

#include <memory>
#include <algorithm>
#include <cstring>

#include "../space/Space.h"
#include "../space/spaceFunction.h"
#include "../iterator/SpaceIterator.h"
#include "../data/Container.h"

template<typename AllocSrc, typename AllocDst>
struct MemCopy;

template<typename Alloc>
struct MemFill;

namespace ma
{
    template
    <
        typename T, typename RangeT, typename Op,
        typename... Args
    >
    void fillArray(T * ptr, T val, space::Space<RangeT> const & space, Op op, Args... args)
    {
        iterator::SpaceIterator<RangeT> it(space);
        while(!it.finish())
        {
            op(ptr + *it, val, it.contiguousDataLength(), args...);
            ++it;
        }
    }

    template
    <
        typename T, typename RangeT, typename Op,
        typename... Args
    >
    void copyArray(T * dst, T const * src, space::Space<RangeT> const & sDst, space::Space<RangeT> const & sSrc, Op & op, Args... args)
    {
        if(!space::shapeEqual(sDst.shape(), sSrc.shape()))
            return;

        SizeT nbrd(
            std::max(
                std::max(
                    sDst.nbNotRangedDim(), sSrc.nbNotRangedDim()),
                1ul
        ));

        SizeT step(std::min(sDst.nbRangedElementAt(nbrd - 1), sSrc.nbRangedElementAt(nbrd - 1)));

        iterator::SpaceIterator<RangeT> itDst(sDst, nbrd, step), itSrc(sSrc, nbrd, step);
        while(!(itDst.finish() || itSrc.finish()))
        {
            op(dst + *itDst, src + *itSrc, itDst.contiguousDataLength(), args...);
            ++itDst; ++itSrc;
        }
    }

    template<typename Data, typename T>
    struct GetAllocator;

    template<typename Range>
    class BaseSpace
    {
    public:
        using SpaceT = space::Space<Range>;
        using BaseT = BaseSpace<Range>;

    protected:
        SpaceT space_;

    public:
        //Default constructor, no allocation
        BaseSpace():space_(){}

        //Allocate but no init with value/vector/iterable
        template<typename L>
        BaseSpace(L && lengths):
            space_(std::forward<L>(lengths))
        {}

        //Same but with const array
        template<typename A, i32 NbDim>
        BaseSpace(A const (&dims)[NbDim]):
            space_(dims)
        {}

        SpaceT space()
        {
            return space_;
        }
    };

    template<typename T, typename Allocator, typename Range>
    class BaseArray : public BaseSpace<Range>
    {
    public:
        using DataT = T;

        using SpaceT = space::Space<Range>;
        using BaseT = BaseSpace<Range>;

        using AllocatorT = Allocator;
        using BaseArrayT = BaseArray<DataT, Allocator, Range>;
        using Container = data::Container<DataT, Allocator>;
        using ContainerPtr = std::shared_ptr<Container>;
        using SpaceIteratorT = iterator::SpaceIterator<Range>;

    private:
        ContainerPtr container_;

    public:
        //Default constructor, no allocation
        BaseArray():BaseT(), container_(){}

        //Allocate but no init with value/vector/iterable
        template<typename L, typename... Args>
        BaseArray(L && lengths, Args... args):
            BaseT(std::forward<L>(lengths)),
            container_(std::make_shared<Container>(BaseT::space_.realSize(), args...))
        {}

        //Same but with const array
        template<typename A, i32 NbDim, typename... Args>
        BaseArray(A const (&dims)[NbDim], Args... args):
            BaseT(dims),
            container_(std::make_shared<Container>(BaseT::space_.realSize(), args...))
        {}

        //Allocate with value/vector/iterable with init
        template<typename L, typename Data, typename... Args>
        BaseArray(L && lengths, Data const & data, Args... args):
            BaseT(std::forward<L>(lengths)),
            container_(std::make_shared<Container>(BaseT::space_.realSize(), args...))
        {
            setMem(data);
        }

        //Same but with const array
        template<typename A, i32 NbDim, typename Data, typename... Args>
        BaseArray(A const (&dims)[NbDim], Data const & data, Args... args):
            BaseT(dims),
            container_(std::make_shared<Container>(BaseT::space_.realSize(), args...))
        {
            setMem(data);
        }

        template<typename OtherArray>
        BaseArray(OtherArray const & oArray):
            BaseT(oArray.space()),
            container_(oArray.container())
        {}

        // template<typename OtherArray>
        // BaseArray(OtherArray && oArray):
        //     BaseT(oArray.space()),
        //     container_(oArray.container())
        // {}

        ~BaseArray(){}

        template<typename OtherArray>
        BaseArrayT & operator=(OtherArray const & oArray)
        {
            return *this;
        }

        template<typename OtherArray>
        BaseArrayT & operator=(OtherArray && oArray)
        {
            return *this;
        }

        BaseArrayT operator[](SizeT pos)
        {
            return ;
        }

        BaseArrayT at(SizeT pos)
        {
            return ;
        }

        template<typename... Args>
        void setMem(DataT val, Args... args)
        {
            fillArray(
                ptr(), val, BaseT::space_,
                MemFill<AllocatorT>::template fill<DataT, Args...>,
                args...
            );
        }

        template<typename OtherArray, typename... Args>
        void setMem(OtherArray const & oArray, Args... args)
        {
            copyArray(
                ptr(), data::ptrOf(oArray), BaseT::space_, spaceOf(oArray),
                MemCopy<AllocatorT, typename GetAllocator<OtherArray, DataT>::type>::template copy<DataT, Args...>,
                args...
            );
        }

        template<typename OtherArray, typename... Args>
        void copyTo(OtherArray & oArray, Args... args)
        {
            copyArray(
                data::ptrOf(oArray), ptr(), spaceOf(oArray), BaseT::space_,
                MemCopy<typename GetAllocator<OtherArray, DataT>::type, AllocatorT>::template copy<DataT, Args...>,
                args...
            );
        }



        DataT* basePtr() const
        {
            return container_->ptr();
        }

        DataT* ptr() const
        {
            return basePtr() + BaseT::space_.baseOffset();
        }

        operator DataT*() const
        {
            return ptr();
        }

        template<typename... Args>
        DataT value() const
        {
            return DataT();
        }

        operator DataT() const
        {
            return value();
        }

    protected:

        template<typename OAllocator>
        SpaceT const & spaceOf(BaseArray<DataT, OAllocator, Range> const & ba) const
        {
            return ba.space();
        }

        template<typename OArray, typename = typename std::enable_if<!std::is_base_of<BaseT, OArray>::value>::type>
        SpaceT const & spaceOf(OArray &oArray)
        {
            return SpaceT(BaseT::space_.baseShape());
        }

    };

    template<typename T, typename Allocator, typename Range>
    struct GetAllocator<BaseArray<T, Allocator, Range>, T>
    {
        using type = typename BaseArray<T, Allocator, Range>::Allocator;
    };

    template<typename Data, typename T>
    struct GetAllocator
    {
        using type = typename std::allocator<T>;
    };

    template<typename T>
    using Array = BaseArray<T, std::allocator<T>, range::LinearRange>;

}

template<typename T>
struct MemCopy<std::allocator<T>, std::allocator<T>>
{
    template<typename DataT>
    static void copy(DataT * dst, DataT const * src, ma::SizeT length)
    {
        std::memcpy(dst, src, length * sizeof(T));
    }
};

template<typename T>
struct MemFill<std::allocator<T>>
{
    template<typename DataT>
    static void fill(DataT * dst, DataT val, ma::SizeT length)
    {
        std::fill_n(dst, length, val);
    }
};

#endif //ARRAY_H
