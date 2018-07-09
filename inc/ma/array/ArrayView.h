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
            using it_traits = std::iterator_traits<Iterator>;

            using value_type =          typename it_traits::value_type;
            using difference_type =     typename it_traits::difference_type;
            using pointer =             typename it_traits::pointer;
            using reference =           typename it_traits::reference;
            using iterator_category =   typename it_traits::iterator_category;

        protected:
            Iterator it_;
            Shape shape_;

        public:
            constexpr explicit ArrayView() noexcept = default;

            template<typename L>
            explicit ArrayView(const L & size) noexcept
                // TODO
            {}


        };
    }
}

#endif //MA_ARRAY_ARRAY_VIEW