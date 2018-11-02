#ifndef MA_ARRAY_ARRAY_VIEW
#define MA_ARRAY_ARRAY_VIEW


#include <ma_api/type.h>
#include <ma_api/traits.h>
#include <ma_api/function.h>
#include <ma_api/printData.h>

#include <ma_api/iterator/ShapeIterator.h>
#include <ma_api/algorithm/copy.h>

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

            using allocator_trait = ma::allocator_traits<allocator_type>;

            using reference         = typename allocator_trait::reference;
            using const_reference         = typename allocator_trait::const_reference;

            using pointer            = typename allocator_trait::pointer;
            using const_pointer      = typename allocator_trait::const_pointer;
            using void_pointer       = typename allocator_trait::void_pointer;
            using const_void_pointer = typename allocator_trait::const_void_pointer;
            using difference_type    = typename allocator_trait::difference_type;
            using size_type          = typename allocator_trait::size_type;
            
            using propagate_on_container_copy_assignment = typename allocator_trait::propagate_on_container_copy_assignment;
            using propagate_on_container_move_assignment = typename allocator_trait::propagate_on_container_move_assignment;
            using propagate_on_container_swap            = typename allocator_trait::propagate_on_container_swap;

            using const_iterator = iterator::ShapeIterator<const_pointer, Shape>;
            using iterator = iterator::ShapeIterator<pointer, Shape>;

        protected:
            Shape shape_;
            pointer ptr_;

        public:
            constexpr ArrayView() noexcept :
                shape_(), ptr_()
            {}

            template<typename L>
            explicit ArrayView(L && size, pointer ptr) noexcept :
                shape_(forward<L>(size)), ptr_(ptr)
            {}

            template<typename Size>
            explicit ArrayView(const std::initializer_list<Size> & size, pointer ptr) noexcept :
                shape_(size), ptr_(ptr)
            {}

            ArrayView(const ArrayView &) = default;
            constexpr ArrayView(ArrayView &&) noexcept = default;
            
            ArrayView& operator=(const ArrayView &) = default;
            ArrayView& operator=(ArrayView &&) noexcept = default;

        protected:
            constexpr explicit ArrayView(Shape && shape, pointer ptr) noexcept :
                shape_(move(shape)), ptr_(ptr)
            {}

        public:
            ~ArrayView() = default;

            template<typename... R>
            ArrayView at(R&&...ranges) const
            {
                return ArrayView(shape_.subShape(forward<R>(ranges)...), ptr_);
            }

            ArrayView operator[](SizeT pos) const
            {
                return ArrayView(shape_.closeAt(pos), ptr_);
            }

            iterator begin()
            {
                return iterator(ptr_, shape_, 0);
            }

            const_iterator begin() const
            {
                return const_iterator(ptr_, shape_, 0);
            }

            iterator end()
            {
                return iterator(ptr_, shape_, size());
            }

            const_iterator end() const
            {
                return const_iterator(ptr_, shape_, size());
            }

            reference val(SizeT pos = 0)
            {
                return reference(ptr_[shape_.at(pos)]);
            }

            const_reference val(SizeT pos = 0) const
            {
                return const_reference(*(ptr_ + shape_.at(pos)));
            }

            pointer ptr() noexcept
            {
                return ptr_ + shape_.at(0);
            }

            constexpr const_pointer ptr() const noexcept
            {
                return ptr_ + shape_.at(0);
            }

            constexpr SizeT size() const noexcept
            {
                return shape_.size();
            }

            constexpr bool empty() const noexcept
            {
                return size() == 0;
            }

            constexpr SizeT step() const noexcept
            {
                return shape_.step();
            }

            VectRange shape() const
            {
                return shape_.shape();
            }

            SizeT ndim() const
            {
                return shape_.ndim();
            }

            constexpr bool contiguous() const noexcept
            {
                return shape_.contiguous();
            }

            template<typename OData, typename... Args>
            void copyTo(OData && data, Args && ... args) const
            {
                algorithm::multiCopy<T>(forward<OData>(data), *this, forward<Args>(args)...);
            }

            template<typename OData, typename... Args>
            void setMem(const OData & data, Args && ... args)
            {
                algorithm::multiCopy<T>(*this, data, forward<Args>(args)...);
            }

            template<typename TT, typename... Args>
            void setMem(const std::initializer_list<TT> & data, Args && ... args)
            {
                algorithm::multiCopy<T>(*this, data, forward<Args>(args)...);
            }
        };

        template <typename T, typename Allocator, typename Shape>
        std::ostream& operator<<(std::ostream& s, const ArrayView<T, Allocator, Shape> & view)
        {
            printData(s, view);
            return s;
        }

    }
}

#endif //MA_ARRAY_ARRAY_VIEW