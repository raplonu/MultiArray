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

            using iterator_category = typename it_traits::iterator_category;
            using value_type = typename it_traits::value_type;
            using difference_type = typename it_traits::difference_type;
            using reference = typename it_traits::reference;
            using pointer = typename it_traits::pointer;

        protected:
            DataT data_;
            SizeT step_;

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
                return *data_;
            }

            value_type operator*() const
            {
                return value();
            }

            StepIteratorT& operator++()
            {
                increment();
                return *this;
            }

            pointer ptr() const
            {
                return &(*data_);
            }

            operator pointer() const
            {
                return ptr();
            }

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
            //
            // using iterator_category = typename it_traits::iterator_category;
            // using value_type = typename it_traits::value_type;
            // using difference_type = typename it_traits::difference_type;
            // using reference = typename it_traits::reference;
            // using pointer = typename it_traits::pointer;

        protected:
            DataT data_;

        public:

            ConstIterator(DataT data):
                data_(data)
            {}

            DataT operator*() const
            {
                return data_;
            }

            ConstIteratorT& operator++()
            {
                return *this;
            }

            ConstIteratorT& operator+=(SizeT step)
            {
                return *this;
            }

            operator DataT() const
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
            return *t;
        }
    }

    namespace iterator
    {
        template<typename T, typename Data>
        auto stepIterator(Data & d, SizeT step)
        -> typename std::enable_if<!std::is_same<T, Data>::value, StepIterator<decltype(std::begin(d))>>::type
        {
            return StepIterator<decltype(std::begin(d))>(std::begin(d), step);
        }

        template<typename T, typename Data>
        auto stepIterator(Data & d, SizeT step)
        -> typename std::enable_if<!std::is_same<T, Data>::value, StepIterator<decltype(&(*d))>>::type
        {
            return StepIterator<decltype(&(*d))>(d, step);
        }

        template<typename T, typename Data>
        auto stepIterator(Data d, SizeT step)
        -> typename std::enable_if< std::is_same<typename std::remove_const<T>::type, Data>::value, ConstIterator<Data>>::type
        {
            return ConstIterator<Data>(d);
        }
    }
}

#endif //MA_ITERATOR_STEP_ITERATOR_H
