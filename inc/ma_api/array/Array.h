#ifndef MA_ARRAY_ARRAY_H
#define MA_ARRAY_ARRAY_H

#include <ma_api/array/ArrayView.h>

namespace ma
{
    namespace array
    {


        template <typename T, typename Container, typename Shape>
        class Array : public ArrayView<T, typename Container::allocator_type, Shape>
        {
            using View = ArrayView<T, typename Container::allocator_type, Shape>;
        public:
            using value_type = T;
            using allocator_type = typename Container::allocator_type;

        protected:
            Container container_;

            template<typename L>
            constexpr Array(Container && container, L && l) noexcept(noexcept(View(std::forward<L>(l), container.data()))) :
                View(std::forward<L>(l), container.data()), container_(std::move(container))
            {}

        public:
            constexpr explicit Array(const allocator_type& allocator = allocator_type()) noexcept :
                View(), container_(allocator)
            {}

            template<typename L, typename = IsNotEquivalent<L, allocator_type>, typename = IsNotEquivalent<L, View>, typename = IsNotEquivalent<L, Array>>
            explicit Array(L && l, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), forward<L>(l))
            {}

            template<typename L>
            explicit Array(const initializer_list<L> & l, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), l)
            {}

            template<typename L, typename Data, typename = IsNotEquivalent<Data, Array>, typename = IsNotEquivalent<L, Container>, typename = IsNotEquivalent<Data, allocator_type>>
            explicit Array(L && l, Data && data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), forward<L>(l))
            {
                View::setMem(std::forward<Data>(data));
            }

            template<typename L, typename Data, typename = IsNotEquivalent<Data, allocator_type>>
            explicit Array(const initializer_list<L> & l, Data && data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), l)
            {
                View::setMem(std::forward<Data>(data));
            }

            template<typename L, typename Data, typename = IsNotEquivalent<L, Container>>
            explicit Array(L && l, const initializer_list<Data> & data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), forward<L>(l))
            {
                View::setMem(data);
            }

            template<typename L, typename Data>
            explicit Array(const initializer_list<L> & l, const initializer_list<Data> & data, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(l), allocator), l)
            {
                View::setMem(data);
            }

            explicit Array(const View & view, const allocator_type& allocator = allocator_type()) noexcept :
                Array(Container(sizeOf(view.shape()), allocator), view.shape())
            {
                View::setMem(view);
            }

            Array(const Array &) = default;
            constexpr Array(Array &&) noexcept = default;

            Array& operator=(const Array &) = default;
            Array& operator=(Array &&) = default;

            constexpr const allocator_type & get_allocator() const
            {
                return container_.get_allocator();
            }

            constexpr SizeT baseSize() const
            {
                return container_.size();
            }

            using View::size;
        };
    }
}

#endif //MA_ARRAY_ARRAY_H