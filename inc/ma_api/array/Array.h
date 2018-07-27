#ifndef MA_ARRAY_ARRAY_H
#define MA_ARRAY_ARRAY_H

#include <ma_api/array/ArrayView.h>

namespace ma
{
    namespace array
    {
        template <typename T, typename Container, typename Shape>
        class Array : private Container, public ArrayView<T, typename Container::allocator_type, Shape>
        {
            using View = ArrayView<T, typename Container::allocator_type, Shape>;
        public:
            using value_type = T;
            using allocator_type = typename Container::allocator_type;

            constexpr explicit Array(const allocator_type& allocator = allocator_type()) noexcept :
                Container(allocator), View()
            {}

            template<typename L>
            explicit Array(L && l, const allocator_type& allocator = allocator_type()) noexcept :
                Container(sizeOf(l), allocator), View(forward<L>(l), Container::data())
            {}

            template<typename L>
            explicit Array(const initializer_list<L> & l, const allocator_type& allocator = allocator_type()) noexcept :
                Container(sizeOf(l), allocator), View(l, Container::data())
            {}

            template<typename L, typename Data, typename = IsNotEquivalent<Data, Array>>
            explicit Array(L && l, Data && data, const allocator_type& allocator = allocator_type()) noexcept :
                Container(sizeOf(l), allocator), View(forward<L>(l), Container::data())
            {
                View::setMem(std::forward<Data>(data));
            }

            template<typename L, typename Data, typename = IsNotEquivalent<Data, Array>>
            explicit Array(const initializer_list<L> & l, Data && data, const allocator_type& allocator = allocator_type()) noexcept :
                Container(sizeOf(l), allocator), View(l, Container::data())
            {
                View::setMem(std::forward<Data>(data));
            }

            template<typename L, typename Data, typename = IsNotEquivalent<Data, Array>>
            explicit Array(L && l, const initializer_list<Data> & data, const allocator_type& allocator = allocator_type()) noexcept :
                Container(sizeOf(l), allocator), View(forward<L>(l), Container::data())
            {
                View::setMem(data);
            }

            template<typename L, typename Data, typename = IsNotEquivalent<Data, Array>>
            explicit Array(const initializer_list<L> & l, const initializer_list<Data> & data, const allocator_type& allocator = allocator_type()) noexcept :
                Container(sizeOf(l), allocator), View(l, Container::data())
            {
                View::setMem(data);
            }

            Array(const Array &) = default;
            constexpr Array(Array &&) noexcept = default;

            Array& operator=(const Array &) = default;
            Array& operator=(Array &&) = default;

            constexpr const allocator_type & get_allocator() const
            {
                return Container::get_allocator();
            }

            constexpr SizeT baseSize() const
            {
                return Container::size();
            }

            using View::size;
        };
    }
}

#endif //MA_ARRAY_ARRAY_H