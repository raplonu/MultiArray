#ifndef MA_ITERATOR_STEP_ITERATOR_H
#define MA_ITERATOR_STEP_ITERATOR_H

#include <vector>
#include <iterator>
#include <algorithm>

#include <ma/detail/base.h>

namespace ma
{
    namespace iterator
    {
        template<typename T>
        class StepIterator
        {
        public:
            using DataT = T;
            using StepIteratorT = StepIterator<DataT>;
            using it_traits = std::iterator_traits<DataT>;

        protected:
            DataT data_;
            SizeT step_;

        public:
            using iterator_category = typename it_traits::iterator_category;
            using value_type = typename it_traits::value_type;
            using difference_type = typename it_traits::difference_type;
            using reference = typename it_traits::reference;
            using pointer = typename it_traits::pointer;


        public:

            StepIterator(DataT data, SizeT step = 1):
                data_(data), step_(step)
            {}

            void increment()
            {
                data_ += step_;
            }

            value_type value() const
            {
                return *ptr();
            }

            reference operator*() const
            {
                return *data_;
            }

            StepIteratorT& operator++()
            {
                increment();
                return *this;
            }

        protected:
            pointer ptr()
            {
                return detail::ptrOf<value_type>(data_);
            }

            const pointer ptr() const
            {
                return detail::ptrOf<const value_type>(data_);
            }

        public:
            // operator pointer()
            // {
            //     return ptr();
            // }
            //
            // operator const pointer() const
            // {
            //     return ptr();
            // }

            bool operator!=(StepIteratorT const & osi)
            {
                return data_ != osi.data_;
            }
        };

        template<typename T>
        class ConstIterator
        {
        public:
            using DataT = T;
            using ConstIteratorT = ConstIterator<DataT>;
            // using it_traits = std::iterator_traits<DataT>;

            using iterator_category = std::forward_iterator_tag;
            using value_type = DataT;
            using difference_type = DiffT;
            using reference = DataT&;
            using pointer = DataT*;

        protected:
            DataT data_;

        public:

            ConstIterator(DataT data):
                data_(data)
            {}

            ConstIteratorT& operator++()
            {
                return *this;
            }

            ConstIteratorT& operator+=(SizeT)
            {
                return *this;
            }

            operator DataT() const
            {
                return data_;
            }

            DataT data() const
            {
                return data_;
            }

            bool operator!=(ConstIteratorT const & osi)
            {
                return data_ != osi.data_;
            }
        };



    }

    namespace detail
    {
        template<typename T>
        T convert(iterator::ConstIterator<T> t)
        {
            return t.data();
        }
    }

    namespace iterator
    {
        template<typename T, typename Data, typename IT = typename std::enable_if<!std::is_same<T, Data>::value, decltype(std::begin(std::declval<Data&>()))>::type>
        StepIterator<IT> stepIterator(Data & d, SizeT step)
        {
            return StepIterator<IT>(std::begin(d), step);
        }

        template<typename T, typename Data, typename StepIt = typename std::enable_if<std::is_same<T, typename std::iterator_traits<Data>::value_type>::value, StepIterator<Data>>::type>
        StepIt stepIterator(Data & d, SizeT step)
        {
            return StepIt(d, step);
        }

        template<typename T>
        StepIterator<T *> stepIterator(T * d, SizeT step)
        {
            return StepIterator<T*>(d, step);
        }

        template<typename T, typename Data, typename = typename std::enable_if< std::is_same<typename std::remove_const<T>::type, Data>::value, ConstIterator<Data>>::type>
        ConstIterator<Data> stepIterator(Data d, SizeT)
        {
            return ConstIterator<Data>(d);
        }
    }
}

namespace std
{
    template<typename RAI>
    struct iterator_traits<ma::iterator::StepIterator<RAI>>
    {
        using StepIteratorT = ma::iterator::StepIterator<RAI>;

        using difference_type =	typename StepIteratorT::difference_type;
        using value_type = typename StepIteratorT::value_type;
        using pointer =	typename StepIteratorT::pointer;
        using reference = typename StepIteratorT::reference;
        using iterator_category = typename StepIteratorT::iterator_category;

    };

    template<typename RAI>
    struct iterator_traits<ma::iterator::ConstIterator<RAI>>
    {
        using ConstIteratorT = ma::iterator::ConstIterator<RAI>;

        using difference_type =	typename ConstIteratorT::difference_type;
        using value_type = typename ConstIteratorT::value_type;
        using pointer =	typename ConstIteratorT::pointer;
        using reference = typename ConstIteratorT::reference;
        using iterator_category = typename ConstIteratorT::iterator_category;

    };
}

#endif //MA_ITERATOR_STEP_ITERATOR_H
