#ifndef MA_TRAITS_H
#define MA_TRAITS_H

#include <type_traits>

#include <ma_api/config.h>
#include <ma_api/type.h>

namespace ma
{
    using std::decay_t;
    using std::enable_if_t;
    using std::add_lvalue_reference_t;
    using std::add_rvalue_reference_t;
    using std::remove_const_t;
    using std::conditional_t;

    #if MA_CXX14

    namespace impl {

    template<bool... Types> struct AllImpl;
    template<bool Head, bool... Types> struct AllImpl<Head, Types...>
    { static constexpr bool value = Head && AllImpl<Types...>::value; };

    template<> struct AllImpl<>
    { static constexpr bool value = true; };

    }

    template<bool... Vals>
    constexpr bool is_all_v = impl::AllImpl<Vals...>::value;

    #else // if MA_CXX17

    template<bool... Vals>
    constexpr bool is_all_v = (... && Vals);

    #endif

    template<bool... Vals>
    using IsAll = enable_if_t<is_all_v<Vals...>>;

    #if not MA_CXX17
    template< class Base, class Derived >
    constexpr bool is_base_of_v = std::is_base_of<Base, Derived>::value;

    template< class T, class U >
    constexpr bool is_same_v = std::is_same<T, U>::value;

    template< class T >
    constexpr bool is_const_v = std::is_const<T>::value;

    template< class T >
    constexpr bool is_integral_v = std::is_integral<T>::value;

    template< class T >
    constexpr bool is_pointer_v = std::is_pointer<T>::value;

    template< class From, class To >
    constexpr bool is_convertible_v = std::is_convertible<From, To>::value;
    #else

    using std::is_base_of_v;
    using std::is_same_v;
    using std::is_const_v;
    using std::is_integral_v;
    using std::is_pointer_v;
    using std::is_convertible_v;

    #endif

    /***************************
     * Standard type traits
     **************************/

    template< class Base, class Derived, typename TT = void>
    using IsBaseOf = enable_if_t<is_base_of_v<Base, Derived>, TT>;

    template< class Base, class Derived, typename TT = void>
    using IsNotBaseOf = enable_if_t<not is_base_of_v<Base, Derived>, TT>;

    template<typename T1, typename T2, typename TT = void>
    using IsSame = enable_if_t<is_same_v<T1, T2>, TT>;

    template<typename T1, typename T2, typename TT = void>
    using IsNotSame = enable_if_t<not is_same_v<T1, T2>, TT>;

    template<typename T, typename TT = void>
    using IsConst = enable_if_t<is_const_v<T>, TT>;

    template<typename T, typename TT = void>
    using IsNotConst = enable_if_t<not is_const_v<T>, TT>;

    template<typename T, typename TT = void>
    using IsIntegral = enable_if_t<is_integral_v<T>, TT>;

    template<typename T, typename TT = void>
    using IsNotIntegral = enable_if_t<not is_integral_v<T>, TT>;

    template<typename T, typename TT = void>
    using IsPointer = enable_if_t<is_pointer_v<T>, TT>;

    template<typename T, typename TT = void>
    using IsNotPointer = enable_if_t<not is_pointer_v<T>, TT>;

    template< class From, class To , typename TT = void>
    using IsConvertible = enable_if_t<is_convertible_v<From, To>, TT>;

    template< class From, class To , typename TT = void>
    using IsNotConvertible = enable_if_t<not is_convertible_v<From, To>, TT>;


    /***************************
     * iterator utility
     **************************/

    template<typename T>
    using ItValueType =     typename std::iterator_traits<T>::value_type;

    template<typename T>
    using ItPointerType =   typename std::iterator_traits<T>::pointer;

    template<typename T>
    using ItReferenceType = typename std::iterator_traits<T>::reference;

    template<typename T>
    using ItDiffType =      typename std::iterator_traits<T>::difference_type;

    template<typename T>
    using ItCategory =      typename std::iterator_traits<T>::iterator_category;


    /***************************
     * allocator utility
     **************************/

    template<typename T>
    using AlValueType =        typename std::allocator_traits<T>::value_type;

    template<typename T>
    using AlPointerType =      typename std::allocator_traits<T>::pointer;

    template<typename T>
    using AlConstPointerType = typename std::allocator_traits<T>::const_pointer;


    /***************************
     * Custom type traits
     **************************/

    template<typename T1, typename T2, typename TT = void>
    constexpr bool is_equivalent_v = is_same_v<decay_t<T1>, decay_t<T2>>;

    template<typename T1, typename T2, typename TT = void>
    using IsEquivalent = enable_if_t<is_equivalent_v<T1, T2>, TT>;

    template<typename T1, typename T2, typename TT = void>
    using IsNotEquivalent = enable_if_t<not is_equivalent_v<T1, T2>, TT>;

    template<typename T, typename TT = void>
    constexpr bool is_bidirect_it_v = is_same_v<ItCategory<T>, std::bidirectional_iterator_tag>;

    template<typename T, typename TT = void>
    using IsBidirectIt = enable_if_t<is_bidirect_it_v<T>, TT>;

    template<typename T, typename TT = void>
    using IsNotBidirectIt = enable_if_t<not is_bidirect_it_v<T>, TT>;

    template<typename T, typename TT = void>
    constexpr bool is_random_it_v = is_same_v<ItCategory<T>, std::random_access_iterator_tag>;

    template<typename T, typename TT = void>
    using IsRandomIt = enable_if_t<is_random_it_v<T>, TT>;

    template<typename T, typename TT = void>
    using IsNotRandomIt = enable_if_t<not is_random_it_v<T>, TT>;

    

    // namespace impl
    // {
    //     template< class T > struct is_raw_pointer_helper     : std::false_type {};
    //     template< class T > struct is_raw_pointer_helper<T*> : std::true_type {};
    // }

    // template<typename T, typename TT = void>
    // using IsRawPointer = enable_if_t<impl::is_raw_pointer_helper<T>::value, TT>;

    // template<typename T, typename TT = void>
    // using IsNotRawPointer = enable_if_t<not impl::is_raw_pointer_helper<T>::value, TT>;

    template<typename Alloc>
    struct allocator_traits : std::allocator_traits<Alloc>
    {
        using Base = std::allocator_traits<Alloc>;

        using value_type = typename Base::value_type;

        using pointer = typename Base::pointer;
        using const_pointer = typename Base::const_pointer;

        template<typename T>
        using DummyRef = decltype(*std::declval<T&>());

        template<typename PTR>
        using Reference = conditional_t<is_same_v<DummyRef<PTR>, value_type>,
                            DummyRef<PTR>,
                            add_lvalue_reference_t<PTR>
                        >;

        using reference = DummyRef<pointer>; // Reference<pointer>;
        using const_reference = DummyRef<const_pointer>; // Reference<const_pointer>;
    };

    /**
     * Contiguous trait
     **/

    namespace impl
    {
        template <typename T>
        auto has_contiguous_met_impl(int) -> decltype (
            std::declval<const T&>().contiguous(),
            std::true_type{});

        template <typename T>
        std::false_type has_contiguous_met_impl(...);
    }

    template <typename T>
    constexpr bool has_contiguous_met_v = decltype(impl::has_contiguous_met_impl<T>(0))::value;

    template<typename T, typename TT = void>
    using HasContiguousMet = enable_if_t<has_contiguous_met_v<T>, TT>;

    template<typename T, typename TT = void>
    using HasNotContiguousMet = enable_if_t<not has_contiguous_met_v<T>, TT>;


    /**
     * Size trait
     **/

    namespace impl
    {
        template <typename T>
        auto has_size_met_impl(int) -> decltype (
            std::declval<const T&>().size(),
            std::true_type{});

        template <typename T>
        std::false_type has_size_met_impl(...);
    }

    template <typename T>
    constexpr bool has_size_met_v = decltype(impl::has_size_met_impl<T>(0))::value;

    template<typename T, typename TT = void>
    using HasSizeMet = enable_if_t<has_size_met_v<T>, TT>;

    template<typename T, typename TT = void>
    using HasNotSizeMet = enable_if_t<not has_size_met_v<T>, TT>;

    /**
     * Step trait
     **/

    namespace impl
    {
        template <typename T>
        auto has_step_met_impl(int) -> decltype (
            std::declval<const T&>().step(),
            std::true_type{});

        template <typename T>
        std::false_type has_step_met_impl(...);
    }

    template <typename T>
    constexpr bool has_step_met_v = decltype(impl::has_step_met_impl<T>(0))::value;

    template<typename T, typename TT = void>
    using HasStepMet = enable_if_t<has_step_met_v<T>, TT>;

    template<typename T, typename TT = void>
    using HasNotStepMet = enable_if_t<not has_step_met_v<T>, TT>;

    /**
     * Shape trait
     **/

    namespace impl
    {
        template <typename T>
        auto has_shape_met_impl(int) -> decltype (
            std::declval<const T&>().shape(),
            std::true_type{});

        template <typename T>
        std::false_type has_shape_met_impl(...);
    }

    template <typename T>
    constexpr bool has_shape_met_v = decltype(impl::has_shape_met_impl<T>(0))::value;

    template<typename T, typename TT = void>
    using HasShapeMet = enable_if_t<has_shape_met_v<T>, TT>;

    template<typename T, typename TT = void>
    using HasNotShapeMet = enable_if_t<not has_shape_met_v<T>, TT>;


    // implementation from https://en.cppreference.com/w/cpp/types/result_of
    namespace impl {
        template <class T>
        struct is_reference_wrapper : std::false_type {};
        template <class U>
        struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};
        
        /**
        * Invoke traits
        **/

        template<class T>
        struct invoke_impl {
            template<class F, class... Args>
            static auto call(F&& f, Args&&... args)
                -> decltype(std::forward<F>(f)(std::forward<Args>(args)...));
        };
        
        template<class B, class MT>
        struct invoke_impl<MT B::*> {
            template<class T, class Td = typename std::decay<T>::type,
                class = typename std::enable_if<std::is_base_of<B, Td>::value>::type
            >
            static auto get(T&& t) -> T&&;
        
            template<class T, class Td = typename std::decay<T>::type,
                class = typename std::enable_if<is_reference_wrapper<Td>::value>::type
            >
            static auto get(T&& t) -> decltype(t.get());
        
            template<class T, class Td = typename std::decay<T>::type,
                class = typename std::enable_if<!std::is_base_of<B, Td>::value>::type,
                class = typename std::enable_if<!is_reference_wrapper<Td>::value>::type
            >
            static auto get(T&& t) -> decltype(*std::forward<T>(t));
        
            template<class T, class... Args, class MT1,
                class = typename std::enable_if<std::is_function<MT1>::value>::type
            >
            static auto call(MT1 B::*pmf, T&& t, Args&&... args)
                -> decltype((invoke_impl::get(std::forward<T>(t)).*pmf)(std::forward<Args>(args)...));
        
            template<class T>
            static auto call(MT B::*pmd, T&& t)
                -> decltype(invoke_impl::get(std::forward<T>(t)).*pmd);
        };
        
        template<class F, class... Args, class Fd = typename std::decay<F>::type>
        auto INVOKE(F&& f, Args&&... args)
            -> decltype(invoke_impl<Fd>::call(std::forward<F>(f), std::forward<Args>(args)...));
        
        template <typename AlwaysVoid, typename, typename...>
        struct invoke_result { };
        template <typename F, typename...Args>
        struct invoke_result<decltype(void(impl::INVOKE(std::declval<F>(), std::declval<Args>()...))),
                        F, Args...> {
            using type = decltype(impl::INVOKE(std::declval<F>(), std::declval<Args>()...));
        };

    } // namespace impl
    
    template <class> struct result_of;

    template <class F, class... ArgTypes>
    struct result_of<F(ArgTypes...)> : impl::invoke_result<void, F, ArgTypes...> {};
    
    template <class F, class... ArgTypes>
    struct invoke_result : impl::invoke_result<void, F, ArgTypes...> {};


    #if MA_CXX17

    using std::invoke_result_t;

    #else

    template< class F, class... ArgTypes>
    using invoke_result_t = typename invoke_result<F, ArgTypes...>::type;

    #endif

}

#endif //MA_TRAITS_H
