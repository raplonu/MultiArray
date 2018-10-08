#ifndef MA_TRAITS_H
#define MA_TRAITS_H

#include <type_traits>

#include <ma_api/config.h>
#include <ma_api/type.h>

namespace ma
{
    #if MA_CXX14
    using std::decay_t;
    using std::enable_if_t;
    using std::add_lvalue_reference_t;
    using std::add_rvalue_reference_t;
    using std::remove_const_t;
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

    template< class T >
    using remove_const_t = typename std::remove_const<T>::type;

    template< bool B, class T, class F >
    using conditional_t = typename std::conditional<B,T,F>::type;
    #endif

    using std::is_base_of;
    using std::is_same;
    using std::is_const;
    using std::is_literal_type;
    using std::is_integral;
    using std::is_pointer;
    using std::is_convertible;

    template< class From, class To , typename TT = void>
    using IsConvertible = enable_if_t<is_convertible<From, To>::value, TT>;

    template< class From, class To , typename TT = void>
    using IsNotConvertible = enable_if_t<not is_convertible<From, To>::value, TT>;

    template<typename T>
    using IteratorCategory = typename std::iterator_traits<T>::iterator_category;

    template<typename T, typename TT = void>
    using IsIntegral = enable_if_t<is_integral<T>::value, TT>;

    template<typename T, typename TT = void>
    using IsNotIntegral = enable_if_t<not is_integral<T>::value, TT>;

    template<typename T>
    using DiffType = typename std::iterator_traits<T>::difference_type ;

    template<typename T>
    using ValueType = typename std::iterator_traits<T>::value_type ;

    template<typename T, typename TT = void>
    using IsConst = enable_if_t<is_const<T>::value, TT>;

    template<typename T, typename TT = void>
    using IsNotConst = enable_if_t<not is_const<T>::value, TT>;

    template<typename T1, typename T2, typename TT = void>
    using IsSame = enable_if_t<is_same<T1, T2>::value, TT>;

    template<typename T1, typename T2, typename TT = void>
    using IsNotSame = enable_if_t<not is_same<T1, T2>::value, TT>;

    template<typename T1, typename T2, typename TT = void>
    using IsEquivalent = IsSame<decay_t<T1>, decay_t<T2>, TT>;

    template<typename T1, typename T2, typename TT = void>
    using IsNotEquivalent = IsNotSame<decay_t<T1>, decay_t<T2>, TT>;

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
        template< class T > struct is_raw_pointer_helper     : std::false_type {};
        template< class T > struct is_raw_pointer_helper<T*> : std::true_type {};
    }

    template<typename T, typename TT = void>
    using IsRawPointer = enable_if_t<impl::is_raw_pointer_helper<T>::value, TT>;

    template<typename T, typename TT = void>
    using IsNotRawPointer = enable_if_t<not impl::is_raw_pointer_helper<T>::value, TT>;

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
        using Reference = conditional_t<SameThanValue<DummyRef<PTR>>::value,
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
    using has_contiguous_met = decltype(impl::has_contiguous_met_impl<T>(0));

    template<typename T, typename TT = void>
    using HasContiguousMet = enable_if_t<has_contiguous_met<T>::value, TT>;

    template<typename T, typename TT = void>
    using HasNotContiguousMet = enable_if_t<not has_contiguous_met<T>::value, TT>;

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
    using has_step_met = decltype(impl::has_step_met_impl<T>(0));

    template<typename T, typename TT = void>
    using HasStepMet = enable_if_t<has_step_met<T>::value, TT>;

    template<typename T, typename TT = void>
    using HasNotStepMet = enable_if_t<not has_step_met<T>::value, TT>;

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
    using has_shape_met = decltype(impl::has_shape_met_impl<T>(0));

    template<typename T, typename TT = void>
    using HasShapeMet = enable_if_t<has_shape_met<T>::value, TT>;

    template<typename T, typename TT = void>
    using HasNotShapeMet = enable_if_t<not has_shape_met<T>::value, TT>;


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
