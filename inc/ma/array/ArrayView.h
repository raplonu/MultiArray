#ifndef MA_ARRAY_ARRAY_VIEW
#define MA_ARRAY_ARRAY_VIEW

namespace ma
{
    namespace array
    {
        template <typename T, typename Allocator, typename Shape>
        class ArrayView
        {
        public:
            using value_type = T;
            using allocator_type = Allocator;

            using allocator_trait = std::allocator_traits<allocator_type>;


            using pointer            = typename allocator_trait::pointer;
            using const_pointer      = typename allocator_trait::const_pointer;
            using void_pointer       = typename allocator_trait::void_pointer;
            using const_void_pointer = typename allocator_trait::const_void_pointer;
            using difference_type    = typename allocator_trait::difference_type;
            using size_type          = typename allocator_trait::size_type;
            
            using propagate_on_container_copy_assignment = typename allocator_trait::propagate_on_container_copy_assignment;
            using propagate_on_container_move_assignment = typename allocator_trait::propagate_on_container_move_assignment;
            using propagate_on_container_swap            = typename allocator_trait::propagate_on_container_swap;

        protected:
            Shape shape_;
            pointer ptr_;

        public:
            constexpr ArrayView() noexcept :
                shape_(), ptr_(nullptr)
            {}

            template<typename L>
            explicit ArrayView(L && size, pointer ptr) noexcept :
                shape_(forward<L>(size)), ptr_(ptr)
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