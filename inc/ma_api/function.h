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

    template< class T, std::size_t N >
    constexpr T* begin( T (&array)[N] ) noexcept { return array; }
    template< class T, std::size_t N >
    constexpr T* end( T (&array)[N] ) noexcept { return array + N; }

    #endif

    namespace impl
    {
        template <typename T>
        auto is_iterable_impl(int)
        -> decltype (
            ma::begin(std::declval<T&>()) != ma::end(std::declval<T&>()), // begin/end and operator !=
            void(), // Handle evil operator ,
            ++std::declval<decltype(ma::begin(std::declval<T&>()))&>(), // operator ++
            void(*ma::begin(std::declval<T&>())), // operator*
            std::true_type{});

        template <typename T>
        std::false_type is_iterable_impl(...);
    }

    template <typename T>
    constexpr bool is_iterable_v = decltype(impl::is_iterable_impl<T>(0))::value;

    template<typename T, typename TT = void>
    using IsIterable = enable_if_t<is_iterable_v<T>, TT>;
    
    template<typename T, typename TT = void>
    using IsNotIterable = enable_if_t<not is_iterable_v<T>, TT>;

    /***************************
     * container utility
     **************************/

    template<typename Container>
    struct container_traits
    {
        using IteratorType = decltype(ma::begin(std::declval<Container&>()));
        using CIteratorType = decltype(ma::begin(std::declval<const Container&>()));

        using value_type = ItValueType<IteratorType>;

        using pointer = ItPointerType<IteratorType>;
        using const_pointer = ItPointerType<CIteratorType>;

        using reference = ItReferenceType<IteratorType>;
        using const_reference = ItReferenceType<CIteratorType>;

        using iterator_category = ItCategory<IteratorType>;
    };

    template<typename T>
    using CoValueType =        typename ma::container_traits<T>::value_type;

    template<typename T>
    using CoPointerType =      typename ma::container_traits<T>::pointer;

    template<typename T>
    using CoConstPointerType = typename ma::container_traits<T>::const_pointer;


    template<typename T, typename TT = void>
    using IsContiguousRange = enable_if_t<is_all_v< is_iterable_v<T>, is_bidirect_it_v<T>>, TT>;

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
    using std::exchange;
    
    /**
     * Absolute function
     **/
    template<typename T>
    constexpr T abs(T t) noexcept { return (t < T{})?-t:t; }

    /**
     * Ceil function
     **/
    template <typename T, typename TT = T>
    constexpr T ceil(const T & a, const TT & b) noexcept { return (a + b - 1) / b; }

    /**
     * Min & Max function
     **/
    using std::max;
    using std::min;

    /**
     * Size function
     **/

    #if MA_CXX17

    using std::size;

    #else

    template < typename C , typename = HasSizeMet<C> > 
    constexpr auto size(const C& c) noexcept { return c.size(); }

    template <class T, std::size_t N>
    constexpr int size(const T (&)[N]) noexcept { return N; }

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
    constexpr bool has_size_v = decltype(impl::has_size_impl<T>(0))::value;

    template<typename T, typename TT = void>
    using HasSize = enable_if_t<has_size_v<T>, TT>;

    template<typename T, typename TT = void>
    using HasNotSize = enable_if_t<not has_size_v<T>, TT>;

    /**
     * Step function
     **/
    template<typename T>
    constexpr auto step(const T & t) noexcept // -> IsIntegral<decltype(t.step()), int>
    { return t.step(); }

    template<typename T, typename = HasNotStepMet<T>>
    constexpr auto step(T const & t) noexcept // -> IsIntegral<decltype(size(t)), int>
    { return size(t); }

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
    constexpr bool has_step_v = decltype(impl::has_step_impl<T>(0))::value;

    template<typename T, typename TT = void>
    using HasStep = enable_if_t<has_step_v<T>, TT>;

    template<typename T, typename TT = void>
    using HasNotStep = enable_if_t<not has_step_v<T>, TT>;

    /**
     * Empty function
     **/
    #if MA_CXX17

    using std::empty;

    #else

    template <class C> 
    constexpr bool empty(const C& c) noexcept { return c.empty(); }

    template <class T, std::size_t N> 
    constexpr bool empty(const T (&)[N]) noexcept { return false; }

    template <class E> 
    constexpr bool empty(std::initializer_list<E> il) noexcept { return il.size() == 0; }

    #endif


    /**
     * Advance function : push forward an iterator n times
     **/
    #if MA_CXX17

    using std::advance;

    #else

    template< class InputIt, typename = IsAll<is_bidirect_it_v<InputIt>, not is_random_it_v<InputIt> > >
    MA_CONSTEXPR void advance(InputIt & it, ItDiffType<InputIt> n)
    {
        if(n >= 0)
            for(int i{}; i < n; ++i)
                ++it;
        else
            for(int i{}; i < -n; ++i)
                --it;
    }

    template< class InputIt>//, typename = IsRandomIt<InputIt>>
    MA_CONSTEXPR void advance(InputIt & it, ItDiffType<InputIt> n)
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
    MA_CONSTEXPR ForwardIt next(ForwardIt it, ItDiffType<ForwardIt> n = 1)
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

    template<typename InputIt, typename = IsNotRandomIt<InputIt> >
    constexpr ItDiffType<InputIt> distance(InputIt t1, InputIt t2) noexcept
    {
        return (t1 != t2) ? 0 : 1 + distance(++t1, t2);
    }

    template<typename InputIt>
    constexpr  ItDiffType<InputIt> distance(InputIt t1, InputIt t2) noexcept
    {
        return t2 - t1;
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
    constexpr T zero() noexcept { return T{}; }

    template<typename T>
    constexpr T one() noexcept { return T{1}; }

    /**
     * Accumulate
     **/
    using std::accumulate;

    /**
     * Contiguous
     **/
    template<typename T, typename = HasContiguousMet<T> >
    auto contiguous(const T & t) noexcept { return t.contiguous(); }

    template<typename T, typename = HasNotContiguousMet<T> >
    constexpr bool contiguous(const T &) noexcept { return true; }

    /**
     * Shape
     **/
    template<typename T, typename = HasShapeMet<T> >
    auto shape(const T & t) noexcept { return t.shape(); }

    template<typename T, typename = HasNotShapeMet<T> >
    VectRange shape(const T & t) noexcept { return VectRange{ma::size(t)}; }

    /**
     * Compare size
     **/

    inline void throwIfMismatch(int s1, int s2, const std::string & str)
    {
        if(s1 != s2) throw std::length_error(str);
    }

    namespace impl
    {

        template<bool DefaultEnabled, typename T, typename = HasSize<T>>
        int tryGetSize(const T & t, int defaultSize)
        {
            int newSize(size(t));

            if(DefaultEnabled) throwIfMismatch(newSize, defaultSize, "Data hasn't the same size");

            return newSize;
        }

        template<bool DefaultEnabled, typename T>
        constexpr auto tryGetSize(T const &, int defaultSize) noexcept -> HasNotSize<T, int>
        {
            return defaultSize;
        }


        template<bool SizeFound, typename... Datas> struct Sizes;

        template<bool SizeFound, typename First, typename... Datas>
        struct Sizes<SizeFound, First, Datas...>
        {
            static int get(int lastSize, const First & f, const Datas &... datas)
            {
                lastSize = tryGetSize<SizeFound>(f, lastSize);

                return Sizes<SizeFound || has_size_v<First>, Datas...>::get
                    (lastSize, datas...);
            }
        };

        //Only for HasSize == true because need to have at least one arg with size
        template<>
        struct Sizes<true>
        {
            constexpr static int get(int lastSize) noexcept { return lastSize; }
        };

    }

    template<typename... T>
    int sizes(const T & ... t)
    {
        return impl::Sizes<false, T...>::get(0, t...);
    }


    /**
     * Steps
     **/

    inline int crossStep(int s1, int s2)
    {
        if(s1>s2) swap(s1, s2);

        // at this point, s1 <= s2

        throwIfMismatch(s2 % s1, 0, "ERROR : Data hasn't the same step");

        return s1;
    }

    namespace impl
    {
        template<bool DefaultEnabled, typename T>
        auto tryGetStep(const T & t, int defaultStep) -> HasStep<T, int>
        {
            if(DefaultEnabled)
                return crossStep(step(t), defaultStep);
            else
                return step(t);
        }

        template<bool DefaultEnabled, typename T>
        constexpr auto tryGetStep(const T &, int defaultStep) noexcept -> HasNotStep<T, int>
        {
            return defaultStep;
        }

        template<bool StepFound, typename... Datas> struct Steps;

        template<bool StepFound, typename First, typename... Datas>
        struct Steps<StepFound, First, Datas...>
        {
            static int get(int lastStep, const First & f, const Datas &... datas)
            {
                lastStep = tryGetStep<StepFound>(f, lastStep);

                return Steps<StepFound || has_step_v<First>, Datas...>::get(lastStep, datas...);
            }
        };

        //Only for has_step == true because need to have at least one arg with size
        template<>
        struct Steps<true>
        {
            constexpr static int get(int lastStep) noexcept { return lastStep; }
        };
    }

    template<typename... T>
    int steps(const T & ... t)
    {
        return impl::Steps<false, T...>::get(0, t...);
    }






















    /**
     * ptrOf function : extract ptr for types
     **/
    template<typename Data, typename = IsPointer<Data>>
    constexpr auto ptrOf(Data && data) noexcept
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
    constexpr bool has_deref_to_ptr_v = decltype(impl::has_deref_to_ptr_impl<T>(0))::value;

    template<typename T, typename TT = void>
    using HasDerefToPtr = enable_if_t<has_deref_to_ptr_v<T>, TT>;

    template<typename T, typename TT = void>
    using HasNotDerefMetToPtr = enable_if_t<not has_deref_to_ptr_v<T>, TT>;

    template<typename Data,
        typename = IsNotPointer<Data>,
        typename = HasDerefToPtr<Data>
    >
    constexpr auto ptrOf(Data && data) noexcept
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

    /**
     * @brief Obtain pointer of an initializer list. Need special implementation due to begin method that return the pointer
     * 
     * @tparam T The underlying data type
     * @param data The initializer list
     * @return const T* the pointer of the underlying array
     */
    template<typename T>
    constexpr const T * ptrOf(const std::initializer_list<T> & data) noexcept
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

    template< typename T, typename Data, typename Res = decltype(ma::ptrOf(std::declval<Data>())),
        typename = IsAll<is_pointer_v<Res>, is_convertible_v<ItValueType<Res>, T> >
    >
    constexpr auto convert(Data && data) noexcept
    {
        return ptrOf(std::forward<Data>(data));
    }

    template<typename T, typename TT , typename = IsAll<is_equivalent_v<T, TT>, is_convertible_v<TT, T> > >
    constexpr TT convert(TT && t) noexcept
    {
        return forward<TT>(t);
    }























    constexpr inline int sizeOf(int l) noexcept
    {
        return l;
    }

    template<typename L, typename = IsIterable<L>>
    int sizeOf(const L & l) noexcept
    {
        return ma::empty(l)
            ? 0
            : accumulate(ma::begin(l), ma::end(l), int(1), std::multiplies<int>());
    }

    namespace impl
    {
        template <typename T>
        auto has_size_of_impl(int) -> decltype (
            sizeOf(std::declval<T&>()),
            std::multiplies<int>()(int(), std::declval<T&>()),
            std::true_type{});

        template <typename T>
        std::false_type has_size_of_impl(...);
    }

    template <typename T>
    using has_size_of_v = decltype(impl::has_size_of_impl<T>(0))::value;


    template<typename T, typename TT = void>
    using HasSizeOf = enable_if_t<has_size_of_v<T>, TT>;

    template<typename T, typename TT = void>
    using HasNotSizeOf = enable_if_t<not has_size_of_v<T>, TT>;

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

    /**
     * Compute the next multiple of b upper than a
     * @param  a start value
     * @param  b value to be multiple of
     * @return   next multiple of b upper than a
     */
    template<typename T>
    inline T nextMul(const T & a, const T & b)
    {
        return ( (a-1) / b + 1) * b;
    }

    template<typename T, typename Pos>
    Pos alignType(Pos offset)
    {
        return nextMul(offset, static_cast<Pos>(std::alignment_of<T>::value));
    }

    template<typename T>
    constexpr void* voidPtr(T * ptr) noexcept { return ptr; }

    template<typename T>
    constexpr const void * voidPtr(const T * ptr) noexcept { return ptr; }

    inline auto makePairer() {
        return [] (const auto & e1, const auto & e2) {
            return std::make_pair(e1, e2);
        };
    }

    template< typename InputIt1, typename InputIt2, typename Map = std::map<ItValueType<InputIt1>, ItValueType<InputIt2>>>
    Map makeMap(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
        Map res;

        std::transform(first1, last1, first2, std::inserter(res, std::end(res)), makePairer());
        
        return res;
    }
}

#endif //MA_FUNCTION_H
