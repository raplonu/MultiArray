#ifndef MA_ARRAY_ARRAY_VIEW
#define MA_ARRAY_ARRAY_VIEW

namespace ma
{
    namespace array
    {
        template <typename T, typename Iterator = T*, typename Shape>
        class ArrayView
        {
        public:
            using value_type = T;


            using it_traits = std::iterator_traits<Iterator>;

            using value_type =          typename it_traits::value_type;
            using difference_type =     typename it_traits::difference_type;
            using pointer =             typename it_traits::pointer;
            using const_pointer =             const typename it_traits::pointer;
            using reference =           typename it_traits::reference;
            using iterator_category =   typename it_traits::iterator_category;

        protected:
            Shape shape_;
            Iterator it_;

        public:
            constexpr explicit ArrayView() noexcept = default;

            template<typename L>
            explicit ArrayView(L && size, Iterator it) noexcept :
                shape_(forward<L>(size)), it_(it)
            {}

            ArrayView(const ArrayView &) = default;
            constexpr ArrayView(ArrayView &&) noexcept = default;
            
            ArrayView& operator=(const ArrayView &) = default;
            ArrayView& operator=(ArrayView &&) noexcept = default;

        protected:
            constexpr explicit ArrayView(Shape && shape, Iterator it) noexcept :
                shape_(move(shape)), it_(it)
            {}

        public:
            ~ArrayView() = default;

            template<typename... R>
            ArrayView at(R&&...ranges) const
            {
                return ArrayView(shape_.subShape(forward<R>(ranges)...), it_);
            }

            ArrayView operator[](SizeT pos) const
            {
                return ArrayView(shape_.subShape(pos), it_);
            }

            constexpr pointer ptr()
            {
                return ptrOf<T>(it_);
            }

            constexpr const_pointer ptr() const
            {
                return ptrOf<T>(it_);
            }


        };
    }
}

#endif //MA_ARRAY_ARRAY_VIEW