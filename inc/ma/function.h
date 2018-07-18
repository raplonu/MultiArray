#ifndef MA_FUNCTION_H
#define MA_FUNCTION_H

#include <numeric>
#include <iterator>
#include <algorithm>

#include <ma/config.h>
#include <ma/type.h>
#include <ma/traits.h>

namespace ma
{
    using std::move;
    using std::forward;

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
    #if MA_CXX14
    using std::begin;
    using std::end; 
    #else
    template< class T, std::size_t N >
    constexpr T* begin( T (&array)[N] ) noexcept { return array; }
    template< class T, std::size_t N >
    constexpr T* end( T (&array)[N] ) noexcept { return array + N; }
    #endif
    #endif

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
     * Absolute function
     **/
    template<typename T>
    constexpr T abs(T t) noexcept
    {
        return (t < 0)?-t:t;
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
    template <typename  C> 
    constexpr SizeT size(const C& c)
    {
        return c.size();
    }

    template <class T, std::size_t N>
    constexpr SizeT size(const T (&)[N]) noexcept
    {
        return N;
    }
    #endif

    /**
     * Empty function
     **/
    #if MA_CXX17
    using std::empty;
    #else
    template <class C> 
    constexpr auto empty(const C& c) -> decltype(c.empty())
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
    constexpr HasNotContigusMet<T, bool> contiguous(T const &) noexcept { return true; }

    template<typename T>
    HasContigusMet<T, bool> contiguous(T const & t) noexcept { return t.contiguous(); }


    /**
     * Compare size
     **/

    inline void compareSize(SizeT s1, SizeT s2)
    {
        if(s1 != s2)
            throw std::length_error("Data hasn't the same size");
    }

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

    namespace impl
    {

        template<bool DefaultEnabled, typename T, typename = HasSize<T>>
        SizeT tryGetSize(const T & t, SizeT defaultSize)
        {
            SizeT newSize(size(t));

            if(DefaultEnabled) compareSize(newSize, defaultSize);

            return newSize;
        }

        template<bool DefaultEnabled, typename T>
        auto tryGetSize(T const &, SizeT defaultSize) ->
        typename std::enable_if<!has_size<T>::value, SizeT>::type
        {
            return defaultSize;
        }


        template<bool SizeFound, typename... Datas> struct Sizes;

        template<bool SizeFound, typename First, typename... Datas>
        struct Sizes<SizeFound, First, Datas...>
        {
            static SizeT get(SizeT lastSize, const First & f, const Datas &... datas)
            {
                SizeT newSize(tryGetSize<SizeFound, First>(f, lastSize));

                return Sizes<SizeFound || has_size<First>::value, Datas...>::get
                    (newSize, datas...);
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
     * ptrOf function : extract ptr for types
     **/
    template<typename T>
    constexpr T * ptrOf(T * data) noexcept
    {
        return data;
    }

    template<typename T, typename Data, typename = IsRandomIt<Data>>
    constexpr T * ptrOf(Data data) noexcept
    {
        return &(*data);
    }

    template<typename T, typename Data>
    auto ptrOf(Data & data) -> decltype(data.data())
    {
        return data.data();
    }

    template<typename T, typename Data>
    auto ptrOf(Data & data) -> decltype(data.ptr())
    {
        return data.ptr();
    }

    template<typename T, typename Data>
    auto ptrOf(Data & data) -> decltype(data.begin())
    {
        return data.begin();
    }

    template<typename T, typename Data>
    auto ptrOf(Data & data) -> decltype(data.get())
    {
        return data.get();
    }

    

    // template<typename T, typename Data, typename = typename enable_pointer<decltype(std::declval<Data&>().begin())>::type>
    // decltype(std::declval<Data&>().begin()) ptrOf(Data & data)
    // {
    //     return data.begin();
    // }

    // template<typename T, typename Data, typename = typename enable_pointer<decltype(std::declval<Data&>().ptr())>::type>
    // decltype(std::declval<Data&>().ptr()) ptrOf(Data & data)
    // {
    //     return data.ptr();
    // }

        
    // template<typename T, typename Data, typename = typename detail::enable_pointer<decltype(&(*std::declval<Data&>()))>::type>
    // decltype(&(*std::declval<Data&>())) ptrOf(Data & data)
    // {
    //     return &(*data);
    // }

    // template<typename T, typename Data, typename = typename detail::enable_pointer<decltype(std::declval<Data&>().data())>::type>
    // decltype(std::declval<Data&>().data()) ptrOf(Data & data)
    // {
    //     return data.data();
    // }

    // template<typename T, typename Data, typename = typename enable_pointer<decltype(std::declval<Data&>().begin())>::type>
    // decltype(std::declval<Data&>().begin()) ptrOf(Data & data)
    // {
    //     return data.begin();
    // }

    // template<typename T, typename Data, typename = typename enable_pointer<decltype(std::declval<Data&>().ptr())>::type>
    // decltype(std::declval<Data&>().ptr()) ptrOf(Data & data)
    // {
    //     return data.ptr();
    // }

    // template<typename T, typename Data, typename = typename enable_pointer<decltype(std::declval<Data&>().get())>::type>
    // decltype(std::declval<Data&>().get()) ptrOf(Data & data)
    // {
    //     return data.get();
    // }


    // template<typename T>
    // auto step(T const & t) -> decltype(t.step())
    // {
    //     return t.step();
    // }

    // template<typename T>
    // auto step(T const & t) ->
    // typename std::enable_if<!has_step_met<T>::value, decltype(size(t))>::type
    // {
    //     return size(t);
    // }

    // namespace impl
    // {
    //     template <typename T>
    //     auto has_step_impl(int) -> decltype (
    //         step(std::declval<T&>()),
    //         std::true_type{});

    //     template <typename T>
    //     std::false_type has_step_impl(...);
    // }

    // template <typename T>
    // using has_step = decltype(impl::has_step_impl<T>(0));

    // inline
    // SizeT crossStep(SizeT s1, SizeT s2)
    // {
    //     SizeT stepMin, stepMax;

    //     if(s1>s2)
    //     {
    //         stepMin = s2; stepMax = s1;
    //     }
    //     else
    //     {
    //         stepMin = s1; stepMax = s2;
    //     }

    //     if(stepMax % stepMin != 0)
    //         throw std::length_error("ERROR : Data hasn't the same step");

    //     return stepMin;
    // }

    // namespace impl
    // {
    //     template<bool DefaultEnabled, typename T>
    //     auto tryGetStep(T const & t, SizeT defaultStep) -> decltype(step(t))
    //     {
    //         if(DefaultEnabled)
    //             return crossStep(step(t), defaultStep);
    //         else
    //             return step(t);
    //     }

    //     template<bool DefaultEnabled, typename T>
    //     auto tryGetStep(T const &, SizeT defaultStep) ->
    //     typename std::enable_if<!has_step<T>::value, SizeT>::type
    //     {
    //         return defaultStep;
    //     }

    //     template<bool StepFound, typename... Datas> struct Steps;

    //     template<bool StepFound, typename First, typename... Datas>
    //     struct Steps<StepFound, First, Datas...>
    //     {
    //         static SizeT get(SizeT lastStep, First const & f, Datas const &... datas)
    //         {
    //             SizeT newStep(tryGetStep<StepFound, First>(f, lastStep));

    //             return Steps<StepFound || has_step<First>::value, Datas...>::get
    //                 (newStep, datas...);
    //         }
    //     };

    //     //Only for has_step == true because need to have at least one arg with size
    //     template<>
    //     struct Steps<true>
    //     {
    //         static SizeT get(SizeT lastStep){ return lastStep; }
    //     };


    // }

    // template<typename... T>
    // SizeT steps(T const & ... t)
    // {
    //     return impl::Steps<false, T...>::get(0, t...);
    // }



    template<typename T>
    constexpr T convert(T t) noexcept
    {
        return t;
    }

    template<typename T>
    constexpr T * convert(T * t) noexcept
    {
        return t;
    }

    template<
        typename T, typename Data,
        typename Ret = IsNotSame<T, Data, decltype(ptrOf<T>(std::declval<Data&>()))>>
    constexpr Ret convert(Data & data) noexcept
    {
        return ptrOf<T>(data);
    }

}

#endif //MA_FUNCTION_H
