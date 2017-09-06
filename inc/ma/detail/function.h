#ifndef MA_DETAIL_FUNCTION_H
#define MA_DETAIL_FUNCTION_H

#include <type_traits>
#include <memory>
#include <vector>
#include <numeric>
#include <iostream>

#include <ma/detail/type.h>
#include <ma/detail/traits.h>

namespace ma
{
    namespace detail
    {

        template<typename T>
        auto size(T const & t) -> decltype(t.size())
        {
            return t.size();
        }

        inline
        void compareSize(SizeT s1, SizeT s2)
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


        namespace impl
        {

            template<bool DefaultEnabled, typename T>
            auto tryGetSize(T const & t, SizeT defaultSize) -> decltype(size(t))
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
                static SizeT get(SizeT lastSize, First const & f, Datas const &... datas)
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
                static SizeT get(SizeT lastSize){ return lastSize; }
            };

        }

        template<typename... T>
        SizeT sizes(T const & ... t)
        {
            return impl::Sizes<false, T...>::get(0, t...);
        }

        template<typename T>
        auto step(T const & t) -> decltype(t.step())
        {
            return t.step();
        }

        template<typename T>
        auto step(T const & t) ->
        typename std::enable_if<!has_step_met<T>::value, decltype(size(t))>::type
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

        inline
        SizeT crossStep(SizeT s1, SizeT s2)
        {
            SizeT stepMin, stepMax;

            if(s1>s2)
            {
                stepMin = s2; stepMax = s1;
            }
            else
            {
                stepMin = s1; stepMax = s2;
            }

            if(stepMax % stepMin != 0)
                throw std::length_error("ERROR : Data hasn't the same step");

            return stepMin;
        }

        namespace impl
        {
            template<bool DefaultEnabled, typename T>
            auto tryGetStep(T const & t, SizeT defaultStep) -> decltype(step(t))
            {
                if(DefaultEnabled)
                    return crossStep(step(t), defaultStep);
                else
                    return step(t);
            }

            template<bool DefaultEnabled, typename T>
            auto tryGetStep(T const &, SizeT defaultStep) ->
            typename std::enable_if<!has_step<T>::value, SizeT>::type
            {
                return defaultStep;
            }

            template<bool StepFound, typename... Datas> struct Steps;

            template<bool StepFound, typename First, typename... Datas>
            struct Steps<StepFound, First, Datas...>
            {
                static SizeT get(SizeT lastStep, First const & f, Datas const &... datas)
                {
                    SizeT newStep(tryGetStep<StepFound, First>(f, lastStep));

                    return Steps<StepFound || has_step<First>::value, Datas...>::get
                        (newStep, datas...);
                }
            };

            //Only for has_step == true because need to have at least one arg with size
            template<>
            struct Steps<true>
            {
                static SizeT get(SizeT lastStep){ return lastStep; }
            };


        }

        template<typename... T>
        SizeT steps(T const & ... t)
        {
            return impl::Steps<false, T...>::get(0, t...);
        }

        template<typename T>//, typename = typename detail::enable_pointer<T>::type>
        T * ptrOf(T * data)
        {
            return data;
        }

        template<typename T, typename Data, typename = typename detail::enable_pointer<decltype(&(*std::declval<Data&>()))>::type>
        decltype(&(*std::declval<Data&>())) ptrOf(Data & data)
        {
            return &(*data);
        }

        template<typename T, typename Data, typename = typename detail::enable_pointer<decltype(std::declval<Data&>().data())>::type>
        decltype(std::declval<Data&>().data()) ptrOf(Data & data)
        {
            return data.data();
        }

        template<typename T, typename Data, typename = typename enable_pointer<decltype(std::declval<Data&>().begin())>::type>
        decltype(std::declval<Data&>().begin()) ptrOf(Data & data)
        {
            return data.begin();
        }

        template<typename T, typename Data, typename = typename enable_pointer<decltype(std::declval<Data&>().ptr())>::type>
        decltype(std::declval<Data&>().ptr()) ptrOf(Data & data)
        {
            return data.ptr();
        }

        // template<typename T, typename Data, typename = typename enable_pointer<decltype(std::declval<Data&>().get())>::type>
        // decltype(std::declval<Data&>().get()) ptrOf(Data & data)
        // {
        //     return data.get();
        // }

        template<typename T>
        T convert(T t)
        {
            return t;
        }

        template<typename T>
        T * convert(T * t)
        {
            return t;
        }

        template<typename T, typename Data, typename Ret = typename std::enable_if<!std::is_same<T, Data>::value, decltype(ptrOf<T>(std::declval<Data&>()))>::type>
        Ret convert(Data & data) //decltype(ptrOf<T>(std::declval<Data&>()))
        {
            return ptrOf<T>(data);
        }

        template<typename T, typename = decltype(std::declval<T&>().isContigous())>
        decltype(std::declval<T&>().isContigous()) isContigous(T const & t)
        {
            return t.isContigous();
        }

        template<typename T, typename = typename std::enable_if<!has_comtigous_met<T>::value, bool>::type>
        bool isContigous(T const &)
        {
            return true;
        }
    }

}

#endif //MA_DETAIL_FUNCTION_H
