#ifndef MA_ITERATOR_STEP_ITERATOR_H
#define MA_ITERATOR_STEP_ITERATOR_H

#include <ma_api/type.h>
#include <ma_api/function.h>

namespace ma
{
    namespace iterator
    {
        template<typename Iterator>
        class StepIterator
        {
            using value_type        = Iterator;
            using pointer           = void;
            using reference         = void;
            using difference_type   = DiffT;
            using iterator_category = std::forward_iterator_tag;

        protected:
            Iterator iterator_;            
            SizeT step_;

        public:
            constexpr StepIterator(Iterator iterator, SizeT step = 1) noexcept :
                iterator_(iterator), step_(step)
            {}

            void increment() noexcept
            {
                iterator_ += step_;
            }

            Iterator operator*() const
            {
                return iterator_;
            }

            StepIterator& operator++()
            {
                increment();
                return *this;
            }

            StepIterator operator++(int)
            {
                StepIterator osi(iterator_, step_);
                increment();
                return osi;
            }

            bool operator!=(const StepIterator & osi)
            {
                return iterator_ != osi.iterator_;
            }
        };

        template<typename T>
        class ConstIterator
        {
        public:

            using value_type        = T;
            using pointer           = void;
            using reference         = void;
            using difference_type   = DiffT;
            using iterator_category = std::input_iterator_tag;

        protected:
            T data_;

        public:

            constexpr explicit ConstIterator(T data) noexcept:
                data_(data)
            {}

            ConstIterator& operator++()
            {
                return *this;
            }

            ConstIterator operator++(int)
            {
                return *this;
            }

            T operator*()
            {
                return data_;
            }

            bool operator!=(const ConstIterator & osi)
            {
                return data_ != osi.data_;
            }
        };



    // }

    // namespace detail
    // {
    //     template<typename T>
    //     T convert(iterator::ConstIterator<T> t)
    //     {
    //         return t.data();
    //     }
    // }

    // namespace iterator
    // {
        template<typename Data, typename Res = IsIterable<Data, StepIterator<decltype(ma::begin(std::declval<Data&>()))>>>
        Res stepIterator(Data && d, SizeT step) noexcept
        {
            return Res(ma::begin(forward<Data>(d)), step);
        }

        // template<typename T, typename Data, typename StepIt = typename std::enable_if<std::is_same<T, typename std::iterator_traits<Data>::value_type>::value, StepIterator<Data>>::type>
        // StepIt stepIterator(Data & d, SizeT step)
        // {
        //     return StepIt(d, step);
        // }

        template<typename Data>
        auto stepIterator(Data && d, SizeT step) noexcept -> IsPointer<Data, StepIterator<Data>>
        {
            return StepIterator<Data>(forward<Data>(d), step);
        }

        template<typename Data, typename = IsNotIterable<Data>, typename Res = IsNotPointer<Data, ConstIterator<Data&>>>
        Res stepIterator(Data && d, SizeT) noexcept
        {
            return Res(forward<Data>(d));
        }
    }
}

// namespace std
// {
//     template<typename RAI>
//     struct iterator_traits<ma::iterator::StepIterator<RAI>>
//     {
//         using StepIteratorT = ma::iterator::StepIterator<RAI>;

//         using difference_type =	typename StepIteratorT::difference_type;
//         using value_type = typename StepIteratorT::value_type;
//         using pointer =	typename StepIteratorT::pointer;
//         using reference = typename StepIteratorT::reference;
//         using iterator_category = typename StepIteratorT::iterator_category;

//     };

//     template<typename RAI>
//     struct iterator_traits<ma::iterator::ConstIterator<RAI>>
//     {
//         using ConstIteratorT = ma::iterator::ConstIterator<RAI>;

//         using difference_type =	typename ConstIteratorT::difference_type;
//         using value_type = typename ConstIteratorT::value_type;
//         using pointer =	typename ConstIteratorT::pointer;
//         using reference = typename ConstIteratorT::reference;
//         using iterator_category = typename ConstIteratorT::iterator_category;

//     };
// }

#endif //MA_ITERATOR_STEP_ITERATOR_H
