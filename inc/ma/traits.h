#ifndef MA_TRAITS_H
#define MA_TRAITS_H

#include <type_traits>

#include <ma/config.h>
#include <ma/type.h>

namespace ma
{
    #if MA_CXX14
    using std::decay_t;
    using std::enable_if_t;
    using std::add_lvalue_reference_t;
    using std::add_rvalue_reference_t;
    using std::conditional_t;

    #else
    template< typename T >
    using decay_t = typename std::decay<T>::type;

    template< bool B, typename T = void >
    using enable_if_t = typename std::enable_if<B,T>::type;

    template< class T >
    using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

    template< class T >
    using add_rvalue_reference_t = typename std::add_rvalue_reference<T>::type;

    template< bool B, class T, class F >
    using conditional_t = typename std::conditional<B,T,F>::type;
    #endif

    using std::is_base_of;
    using std::is_same;
    using std::is_const;
    using std::is_literal_type;
    using std::is_integral;
    using std::is_pointer;

    template<typename T, typename TT = void>
    using IsIntegral = enable_if_t<is_integral<T>::value, TT>;

    template<typename T, typename TT = void>
    using IsNotIntegral = enable_if_t<not is_integral<T>::value, TT>;

    template<typename T1, typename T2>
    using IsNotEquivalent = enable_if_t<not is_same<decay_t<T1>, decay_t<T2>>::value>;

    template<typename T>
    using IteratorCategory = typename std::iterator_traits<T>::iterator_category;

    template<typename T>
    using DiffType = typename std::iterator_traits<T>::difference_type ;

    template<typename T, typename TT = void>
    using IsConst = enable_if_t<is_const<T>::value, TT>;

    template<typename T, typename TT = void>
    using IsNotConst = enable_if_t<not is_const<T>::value, TT>;

    template<typename T1, typename T2, typename TT = void>
    using IsSame = enable_if_t<is_same<T1, T2>::value, TT>;

    template<typename T1, typename T2, typename TT = void>
    using IsNotSame = enable_if_t<not is_same<T1, T2>::value, TT>;

    template<typename T, typename TT = void>
    using IsBidirectIt = enable_if_t<is_same<IteratorCategory<T>, std::bidirectional_iterator_tag>::value, TT>;

    template<typename T, typename TT = void>
    using IsNotBidirectIt = enable_if_t<not is_same<IteratorCategory<T>, std::bidirectional_iterator_tag>::value, TT>;

    template<typename T, typename TT = void>
    using IsRandomIt = enable_if_t<is_same<IteratorCategory<T>, std::random_access_iterator_tag>::value, TT>;

    template<typename T, typename TT = void>
    using IsNotRandomIt = enable_if_t<not is_same<IteratorCategory<T>, std::random_access_iterator_tag>::value, TT>;

    template<typename T, typename TT = void>
    using IsPointer = enable_if_t<is_pointer<T>::value, TT>;

    template<typename T, typename TT = void>
    using IsNotPointer = enable_if_t<not is_pointer<T>::value, TT>;

    namespace impl
    {
        template <typename T>
        auto is_iterable_impl(int)
        -> decltype (
            begin(std::declval<T&>()) != end(std::declval<T&>()), // begin/end and operator !=
            void(), // Handle evil operator ,
            ++std::declval<decltype(begin(std::declval<T&>()))&>(), // operator ++
            void(*begin(std::declval<T&>())), // operator*
            std::true_type{});

        template <typename T>
        std::false_type is_iterable_impl(...);
    }

    template <typename T>
    using is_iterable = decltype(impl::is_iterable_impl<T>(0));

    template<typename T, typename TT = void>
    using IsIterable = enable_if_t<is_iterable<T>::value, TT>;
    
    template<typename T, typename TT = void>
    using IsNotIterable = enable_if_t<not is_iterable<T>::value, TT>;


    template<typename Alloc>
    struct allocator_traits : std::allocator_traits<Alloc>
    {
        using Base = std::allocator_traits<Alloc>;

        using value_type = typename Base::value_type;

        using pointer = typename Base::pointer;
        using const_pointer = typename Base::const_pointer;

        template<typename T>
        using DummyRef = decltype(*std::declval<T&>());

        template<typename T>
        using SameThanValue = is_same<T, value_type>;

        template<typename PTR>
        using Reference = conditional_t<SameThanValue<DummyRef<PTR>>::value, DummyRef<PTR>, add_lvalue_reference_t<PTR>>;

        using reference = Reference<pointer>;
        using const_reference = Reference<const_pointer>;
    };

    // namespace impl
    // {
    //     template <typename T>
    //     auto has_size_method_impl(int) -> decltype (
    //         std::declval<T&>().size(),
    //         std::true_type{});

    //     template <typename T>
    //     std::false_type has_size_method_impl(...);
    // }

    // template <typename T>
    // using has_size_method = decltype(impl::has_size_method_impl<T>(0));

    // namespace impl
    // {
    //     template <typename T>
    //     auto has_step_met_impl(int) -> decltype (
    //         std::declval<T&>().step(),
    //         std::true_type{});

    //     template <typename T>
    //     std::false_type has_step_met_impl(...);
    // }

    // template <typename T>
    // using has_step_met = decltype(impl::has_step_met_impl<T>(0));

    // namespace impl
    // {
    //     template <typename T>
    //     auto has_comtigous_met_impl(int) -> decltype (
    //         std::declval<T&>().isContigous(),
    //         std::true_type{});

    //     template <typename T>
    //     std::false_type has_comtigous_met_impl(...);
    // }

    // template <typename T>
    // using has_comtigous_met = decltype(impl::has_comtigous_met_impl<T>(0));

    

    // //Detect full contiguous alloc like raw ptr, vector, array
    // template<typename T>
    // using is_contiguous_alloc = typename std::is_same
    // <
    //     typename std::iterator_traits<typename T::iterator>::iterator_category,
    //     std::random_access_iterator_tag
    // >;

    // //Detect not contiguous alloc like list, map
    // template<typename T>
    // using is_sequence_alloc = typename std::is_same
    // <
    //     typename std::iterator_traits<typename T::iterator>::iterator_category,
    //     std::bidirectional_iterator_tag
    // >;

    // namespace impl
    // {
    //     template <typename T, typename Default>
    //     auto get_allocator_type_impl(int) -> typename T::allocator_type;

    //     template <typename T, typename Default>
    //     Default get_allocator_type_impl(...);
    // }

    // template <typename T, typename Default>
    // struct get_allocator_type{using type = decltype(impl::get_allocator_type_impl<T, Default>(0));};

    // namespace impl
    // {
    //     template <typename T, typename Alloc>
    //     auto has_same_alloc_impl(int) -> decltype (
    //         std::is_same<typename T::AllocT&, Alloc>::value()
    //     );

    //     template <typename T, typename Alloc>
    //     std::false_type has_same_alloc_impl(...);
    // }

    // template<typename T, typename Alloc>
    // using has_same_alloc = decltype(impl::has_same_alloc_impl<T, Alloc>(0));

    // template<typename T>
    // using enable_pointer = std::enable_if<std::is_pointer<T>::value, T>;
}



























//         namespace impl
//         {

//
//             template <typename T, typename Default>
//             auto get_allocator_type_impl(int) -> typename T::allocator_type;
//
//             template <typename T, typename Default>
//             Default get_allocator_type_impl(...);
//
//             template <typename T>
//             auto has_size_impl(int) -> decltype (
//                 std::declval<T&>().size(),
//                 std::true_type{});
//
//             template <typename T>
//             std::false_type has_size_impl(...);
//
//             template <typename T>
//             auto has_size_impl(int)
//             -> decltype (
//                 size(std::declval<T&>()),
//                 std::true_type{});
//
//             template <typename T>
//             std::false_type has_size_impl(...);
//
//             template <typename PtrT, typename T>
//             auto has_ptr_impl(int)
//             -> decltype (
//                 common::ptrOf<PtrT>(std::declval<T&>()),
//                 std::true_type{});
//
//             template <typename PtrT, typename T>
//             std::false_type has_ptr_impl(...);
//         }
//

//
//         template <typename T>
//         using has_size = decltype(impl::has_size_impl<T>(0));
//
//         template <typename PtrT, typename T>
//         using has_ptr = decltype(impl::has_ptr_impl<PtrT, T>(0));
//
//         template <typename Data, typename T, typename Allocator>
//         struct enable_if_not_superbase : public std::enable_if
//         <
//             !( std::is_base_of<base::BaseForceCopy<T>, Data>::value
//             || std::is_base_of<BaseData<T, Allocator>, Data>::value)
//         >{};
//
//
//         template <typename T, typename DataT>
//         struct get_allocator_type_or_default : public get_allocator_type<T, DefaultAlloc<DataT> >{};
//
//         template<typename T, class U = void>
//         struct enable_if_is_iterable : public std::enable_if<is_iterable<T>::value>{};
//
//         template<typename T, class U = void>
//         struct enable_if_integral : public std::enable_if<std::is_integral<T>::value>{};
//
//         template<typename T, class U = void>
//         struct enable_if_has_size : public std::enable_if<std::has_size<T>::value>{};
//
//         template<typename Base, typename T>
//         struct enable_if_base_of : public std::enable_if<std::is_base_of<Base, T>::value>{};
//
//         template<typename Base, typename T>
//         struct enable_if_not_base_of : public std::enable_if<!std::is_base_of<Base, T>::value>{};
//
//         template<typename Base, typename T>
//         struct enable_if_not_same : public std::enable_if<!std::is_same<Base, T>::value>{};
//
//         template<typename T, class U = void>
//         struct enable_if_not_ptr : public std::enable_if<!std::is_pointer<T>::value>{};
//
//         //ptr or value
//         template<typename T, typename DataT, class U = void>
//         struct enable_if_not_simple_type : public std::enable_if<!(std::is_pointer<T>::value || std::is_same<T, DataT>::value)>{};
//     }
// }

#endif //MA_TRAITS_H
