#ifndef MA_FUNCTION_H
#define MA_FUNCTION_H

#include <numeric>
#include <iterator>
#include <functional>
#include <algorithm>
#include <map>

#include <ma_api/config.h>
#include <ma_api/type.h>
#include <ma_api/traits.h>

namespace ma
{
    using std::move;
    using std::forward;
    using std::initializer_list;

    /**
     * Begin & End function
     **/
    #if MA_CXX17
        using std::begin;
        using std::end;
    #else
        template< class C , typename = IsNotConst<C>>
        constexpr auto begin( C& c ) noexcept -> decltype(c.begin()) { return c.begin(); }
        template< class C >
        constexpr auto begin( const C& c ) noexcept -> decltype(c.begin()) { return c.begin(); }

        template< class C , typename = IsNotConst<C>>
        constexpr auto end( C& c ) noexcept -> decltype(c.end()) { return c.end(); }
        template< class C >
        constexpr auto end( const C& c ) noexcept -> decltype(c.end()) { return c.end(); }
        // #if MA_CXX14
        //     using std::begin;
        //     using std::end; 
        // #else
            template< class T, std::size_t N >
            constexpr T* begin( T (&array)[N] ) noexcept { return array; }
            template< class T, std::size_t N >
            constexpr T* end( T (&array)[N] ) noexcept { return array + N; }
        // #endif
    #endif

    template<typename Container>
    using ContainerValueType = ValueType<decltype(ma::begin(std::declval<Container&>()))>;

    /**
     * Front & Back function
     **/
    template<typename C>
    constexpr auto front(C & c) -> decltype(c.front()) { return c.front(); }

    template<typename C>
    constexpr auto front(const C & c) -> decltype(c.front()) { return c.front(); }

    template<typename C>
    constexpr auto back(C & c) -> decltype(c.back()) { return c.back(); }
    
    template<typename C>
    constexpr auto back(const C & c) -> decltype(c.back()) { return c.back(); }

    template< class T, std::size_t N >
    constexpr T & front( T (&array)[N] ) noexcept { return array[0]; }

    template< class T, std::size_t N >
    constexpr const T & front( const T (&array)[N] ) noexcept { return array[0]; }

    template< class T, std::size_t N >
    constexpr T & back( T (&array)[N] ) noexcept { return array[N - 1]; }

    template< class T, std::size_t N >
    constexpr const T & back( const T (&array)[N] ) noexcept { return array[N - 1]; }

    /**
     * Exchange function
     **/
    #if MA_CXX14
    using std::exchange;
    #else
    template<class T, class U = T>
    T exchange(T& obj, U&& new_value)
    {
        T old_value = std::move(obj);
        obj = std::forward<U>(new_value);
        return old_value;
    }
    #endif
    
    /**
     * Absolute function
     **/
    template<typename T>
    constexpr T abs(T t) noexcept
    {
        return (t < 0)?-t:t;
    }

    /**
     * Ceil function
     **/
    template <typename T, typename TT = T>
    constexpr T ceil(const T & a, const TT & b) noexcept
    {
        return (a + b - 1) / b;
    }

    /**
     * Min & Max function
     **/

    #if MA_CXX14
    using std::max;
    using std::min;
    #else
    template<typename T>
    constexpr const T & max(const T & t1, const T & t2) noexcept
    {
        return (t1 > t2)?t1:t2;
    }

    template<typename T>
    constexpr const T & min(const T & t1, const T & t2) noexcept
    {
        return (t1 < t2)?t1:t2;
    }
    #endif

    /**
     * Size function
     **/

    #if MA_CXX17
    using std::size;
    #else
    template <typename C> 
    constexpr auto size(const C& c) noexcept -> IsIntegral<decltype(c.size()), SizeT>
    {
        return c.size();
    }

    template <class T, std::size_t N>
    constexpr SizeT size(const T (&)[N]) noexcept
    {
        return N;
    }
    #endif

    namespace impl
    {
        template <typename T>
        auto has_size_impl(int) -> decltype (
            size(std::declval<T&>()),
            std::true_type{});

        template <typename T>
        std::false_type has_size_impl(...);
    }

    template <typename T>
    using has_size = decltype(impl::has_size_impl<T>(0));


    template<typename T, typename TT = void>
    using HasSize = enable_if_t<has_size<T>::value, TT>;

    template<typename T, typename TT = void>
    using HasNotSize = enable_if_t<not has_size<T>::value, TT>;

    /**
     * Step function
     **/
    template<typename T>
    constexpr auto step(const T & t) noexcept -> IsIntegral<decltype(t.step()), SizeT>
    {
        return t.step();
    }

    template<typename T, typename = HasNotStepMet<T>>
    constexpr auto step(T const & t) noexcept -> IsIntegral<decltype(size(t)), SizeT>
    {
        return size(t);
    }

    namespace impl
    {
        template <typename T>
        auto has_step_impl(int) -> decltype (
            step(std::declval<T&>()),
            std::true_type{});

        template <typename T>
        std::false_type has_step_impl(...);
    }

    template <typename T>
    using has_step = decltype(impl::has_step_impl<T>(0));


    template<typename T, typename TT = void>
    using HasStep = enable_if_t<has_step<T>::value, TT>;

    template<typename T, typename TT = void>
    using HasNotStep = enable_if_t<not has_step<T>::value, TT>;

    /**
     * Empty function
     **/
    #if MA_CXX17
    using std::empty;
    #else
    template <class C> 
    constexpr auto empty(const C& c) noexcept -> decltype(c.empty())
    {
        return c.empty();
    }

    template <class T, std::size_t N> 
    constexpr bool empty(const T (&)[N]) noexcept
    {
        return false;
    }

    template <class E> 
    constexpr bool empty(std::initializer_list<E> il) noexcept
    {
        return il.size() == 0;
    }
    #endif    


    /**
     * Advance function : push forward an iterator n times
     **/
    #if MA_CXX17
    using std::advance;
    #else
    template< class InputIt, class Distance, typename = IsBidirectIt<InputIt>, typename = IsNotRandomIt<InputIt>>
    CONSTEXPR14 void advance(InputIt & it, Distance n)
    {
        if(n >= 0)
            for(SizeT i(0); i < n; ++i)
                ++it;
        else
            for(SizeT i(0); i < -n; ++i)
                --it;
    }

    template< class InputIt, class Distance, typename = IsRandomIt<InputIt>>
    CONSTEXPR14 void advance(InputIt & it, Distance n)
    {
        it += n;
    }
    #endif


    /**
     * Next function : return it + n
     **/

    #if MA_CXX17
    using std::next;
    #else
    template<class ForwardIt>
    CONSTEXPR14 ForwardIt next(ForwardIt it, DiffType<ForwardIt> n = 1)
    {
        ma::advance(it, n);
        return it;
    }
    #endif

    /**
     * Distance function
     **/
    #if MA_CXX17
    using std::distance;
    #else
    template<typename T>
    constexpr  IsRandomIt<T, SizeT> distance(const T & t1, const T & t2) noexcept
    {
        return static_cast<SizeT>(t2 - t1);
    }

    template<typename T>
    constexpr IsNotRandomIt<T, SizeT> distance(T t1, T t2) noexcept
    {
        //Force to use recurtion because of c++11 constexpr limitation
        return (t1 != t2) ? 0 : distance(++t1, t2) + 1;
    }
    #endif

    /**
     * Copy function
     **/
    using std::copy;
    using std::copy_n;
    using std::fill;
    using std::fill_n;

    /**
     * Swap function
     **/
    using std::swap;

    /**
     * Special Values
     **/
    template<typename T>
    constexpr T zero() noexcept
    {
        return T(0);
    }

    template<typename T>
    constexpr T one() noexcept
    {
        return T(1);
    }

    /**
     * Accumulate
     **/
    using std::accumulate;

    /**
     * Contiguous
     **/
    template<typename T>
    constexpr HasNotContiguousMet<T, bool> contiguous(T const &) noexcept { return true; }

    template<typename T>
    HasContiguousMet<T, bool> contiguous(T const & t) noexcept { return t.contiguous(); }

    /**
     * Shape
     **/
    template<typename T>
    constexpr HasNotShapeMet<T, VectRange> shape(T const & t) noexcept { return VectRange{ma::size(t)}; }

    template<typename T>
    HasShapeMet<T, VectRange> shape(T const & t) noexcept { return t.shape(); }


    /**
     * Compare size
     **/

    inline void throwIfMismatch(SizeT s1, SizeT s2, const std::string & str)
    {
        if(s1 != s2) throw std::length_error(str);
    }

    namespace impl
    {

        template<bool DefaultEnabled, typename T, typename = HasSize<T>>
        SizeT tryGetSize(const T & t, SizeT defaultSize)
        {
            SizeT newSize(size(t));

            if(DefaultEnabled) throwIfMismatch(newSize, defaultSize, "Data hasn't the same size");

            return newSize;
        }

        template<bool DefaultEnabled, typename T>
        constexpr auto tryGetSize(T const &, SizeT defaultSize) noexcept -> HasNotSize<T, SizeT>
        {
            return defaultSize;
        }


        template<bool SizeFound, typename... Datas> struct Sizes;

        template<bool SizeFound, typename First, typename... Datas>
        struct Sizes<SizeFound, First, Datas...>
        {
            static SizeT get(SizeT lastSize, const First & f, const Datas &... datas)
            {
                lastSize = tryGetSize<SizeFound>(f, lastSize);

                return Sizes<SizeFound || has_size<First>::value, Datas...>::get
                    (lastSize, datas...);
            }
        };

        //Only for HasSize == true because need to have at least one arg with size
        template<>
        struct Sizes<true>
        {
            constexpr static SizeT get(SizeT lastSize) noexcept { return lastSize; }
        };

    }

    template<typename... T>
    SizeT sizes(const T & ... t)
    {
        return impl::Sizes<false, T...>::get(0, t...);
    }


    /**
     * Steps
     **/

    inline SizeT crossStep(SizeT s1, SizeT s2)
    {
        if(s1>s2) swap(s1, s2);

        // at this point, s1 <= s2

        throwIfMismatch(s2 % s1, 0, "ERROR : Data hasn't the same step");

        return s1;
    }

    namespace impl
    {
        template<bool DefaultEnabled, typename T>
        auto tryGetStep(const T & t, SizeT defaultStep) -> HasStep<T, SizeT>
        {
            if(DefaultEnabled)
                return crossStep(step(t), defaultStep);
            else
                return step(t);
        }

        template<bool DefaultEnabled, typename T>
        constexpr auto tryGetStep(const T &, SizeT defaultStep) noexcept -> HasNotStep<T, SizeT>
        {
            return defaultStep;
        }

        template<bool StepFound, typename... Datas> struct Steps;

        template<bool StepFound, typename First, typename... Datas>
        struct Steps<StepFound, First, Datas...>
        {
            static SizeT get(SizeT lastStep, const First & f, const Datas &... datas)
            {
                lastStep = tryGetStep<StepFound>(f, lastStep);

                return Steps<StepFound || has_step<First>::value, Datas...>::get(lastStep, datas...);
            }
        };

        //Only for has_step == true because need to have at least one arg with size
        template<>
        struct Steps<true>
        {
            constexpr static SizeT get(SizeT lastStep) noexcept { return lastStep; }
        };
    }

    template<typename... T>
    SizeT steps(const T & ... t)
    {
        return impl::Steps<false, T...>::get(0, t...);
    }

    /**
     * ptrOf function : extract ptr for types
     **/
    template<typename Data>
    constexpr auto ptrOf(Data && data) noexcept -> IsPointer<Data, Data>
    {
        return data;
    }

    namespace impl
    {
        template <typename T>
        auto has_deref_to_ptr_impl(int) -> decltype (
            IsPointer<decltype(&(*std::declval<T&>())), int>{},
            std::true_type{});

        template <typename T>
        std::false_type has_deref_to_ptr_impl(...);
    }

    template<typename T>
    using has_deref_to_ptr = decltype(impl::has_deref_to_ptr_impl<T>(0));

    template<typename T, typename TT = void>
    using HasDerefToPtr = enable_if_t<has_deref_to_ptr<T>::value, TT>;

    template<typename T, typename TT = void>
    using HasNotDerefMetToPtr = enable_if_t<not has_deref_to_ptr<T>::value, TT>;

    template<typename Data,
        typename = IsNotPointer<Data>
    >
    constexpr auto ptrOf(Data && data) noexcept -> HasDerefToPtr<Data, decltype(&(*data))>
    {
        return &(*data);
    }

    namespace impl
    {
        template <typename T>
        auto has_data_met_to_ptr_impl(int) -> decltype (
            IsPointer<decltype(std::declval<T&>().data()), int>{},
            std::true_type{});

        template <typename T>
        std::false_type has_data_met_to_ptr_impl(...);
    }

    template<typename T>
    using has_data_met_to_ptr = decltype(impl::has_data_met_to_ptr_impl<T>(0));

    template<typename T, typename TT = void>
    using HasDataMetToPtr = enable_if_t<has_data_met_to_ptr<T>::value, TT>;

    template<typename T, typename TT = void>
    using HasNotDataMetToPtr = enable_if_t<not has_data_met_to_ptr<T>::value, TT>;

    template<typename Data,
        typename = IsNotPointer<Data>,
        typename = HasNotDerefMetToPtr<Data>
    >
    constexpr auto ptrOf(Data && data) noexcept -> HasDataMetToPtr<Data, decltype(data.data())>
    {
        return data.data();
    }

    namespace impl
    {
        template <typename T>
        auto has_ptr_met_to_ptr_impl(int) -> decltype (
            IsPointer<decltype(std::declval<T&>().ptr()), int>{},
            decltype(std::declval<T&>().ptr()){},
            std::true_type{});

        template <typename T>
        std::false_type has_ptr_met_to_ptr_impl(...);
    }

    template<typename T>
    using has_ptr_met_to_ptr = decltype(impl::has_ptr_met_to_ptr_impl<T>(0));

    template<typename T, typename TT = void>
    using HasPtrMetToPtr = enable_if_t<has_ptr_met_to_ptr<T>::value, TT>;

    template<typename T, typename TT = void>
    using HasNotPtrMetToPtr = enable_if_t<not has_ptr_met_to_ptr<T>::value, TT>;

    template<typename Data,
        typename = IsNotPointer<Data>,
        typename = HasNotDerefMetToPtr<Data>,
        typename = HasNotDataMetToPtr<Data>
    >
    constexpr auto ptrOf(Data && data) noexcept -> HasPtrMetToPtr<Data, decltype(data.ptr())>
    {
        return data.ptr();
    }

    namespace impl
    {
        template <typename T>
        auto has_begin_met_to_ptr_impl(int) -> decltype (
            IsPointer<decltype(std::declval<T&>().begin()), int>{},
            std::true_type{});

        template <typename T>
        std::false_type has_begin_met_to_ptr_impl(...);
    }

    template<typename T>
    using has_begin_met_to_ptr = decltype(impl::has_begin_met_to_ptr_impl<T>(0));

    template<typename T, typename TT = void>
    using HasBeginMetToPtr = enable_if_t<has_begin_met_to_ptr<T>::value, TT>;

    template<typename T, typename TT = void>
    using HasNotBeginMetToPtr = enable_if_t<not has_begin_met_to_ptr<T>::value, TT>;

    template<typename Data,
        typename = IsNotPointer<Data>,
        typename = HasNotDerefMetToPtr<Data>,
        typename = HasNotDataMetToPtr<Data>,
        typename = HasNotPtrMetToPtr<Data>
    >
    constexpr auto ptrOf(Data && data) noexcept -> HasBeginMetToPtr<Data, decltype(data.begin())>
    {
        return data.begin();
    }

    namespace impl
    {
        template <typename T>
        auto has_get_met_to_ptr_impl(int) -> decltype (
            IsPointer<decltype(std::declval<T&>().get()), int>{},
            std::true_type{});

        template <typename T>
        std::false_type has_get_met_to_ptr_impl(...);
    }

    template<typename T>
    using has_get_met_to_ptr = decltype(impl::has_get_met_to_ptr_impl<T>(0));

    template<typename T, typename TT = void>
    using HasGetMetToPtr = enable_if_t<has_get_met_to_ptr<T>::value, TT>;

    template<typename T, typename TT = void>
    using HasNotGetMetToPtr = enable_if_t<not has_get_met_to_ptr<T>::value, TT>;

    template<typename Data,
        typename = IsNotPointer<Data>,
        typename = HasNotDerefMetToPtr<Data>,
        typename = HasNotDataMetToPtr<Data>,
        typename = HasNotPtrMetToPtr<Data>,
        typename = HasNotBeginMetToPtr<Data>
    >
    constexpr auto ptrOf(Data && data) noexcept -> HasGetMetToPtr<Data, decltype(data.get())>
    {
        return data.get();
    }

    template<typename T, typename Data, typename TT = void>
    using HasRawPtr = IsEquivalent<T*, decltype(ptrOf(std::declval<Data&>())), TT>;

    template<typename T, typename Data, typename TT = void>
    using HasNotRawPtr = IsNotEquivalent<T*, decltype(ptrOf(std::declval<Data&>())), TT>;


    /**
     * Convert
     **/

    template<typename T, typename TT , typename = IsEquivalent<T, TT>>
    constexpr auto convert(TT && t) noexcept -> decltype(forward<TT>(t))
    {
        return forward<TT>(t);
    }

    template< typename T, typename Data >
    constexpr auto convert(Data && data) noexcept -> decltype(ptrOf(std::forward<Data>(data)))
    {
        return ptrOf(std::forward<Data>(data));
    }

    constexpr inline SizeT sizeOf(SizeT l) noexcept
    {
        return l;
    }

    template<typename L, typename = IsIterable<L>>
    SizeT sizeOf(const L & l) noexcept
    {
        return ma::empty(l)
            ? 0
            : accumulate(ma::begin(l), ma::end(l), SizeT(1), std::multiplies<SizeT>());
    }

    template<typename> struct ProxyPtrValid;

    template<typename T>
    struct ProxyPtrValid<T *>
    {
        static constexpr bool valid(T * ptr) noexcept
        {
            return (bool)ptr;
        }
    };

    template<typename T>
    constexpr bool ptrValid(const T & ptr) noexcept
    {
        return ProxyPtrValid<T>::valid(ptr);
    }
}

#endif //MA_FUNCTION_H
