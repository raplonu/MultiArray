#ifndef FUNCTION_H
#define FUNCTION_H

#include <initializer_list>
#include <memory>
#include <vector>

#include "traits.h"

namespace ma
{
    namespace common
    {
        template<typename BeginIt, typename EndIt, typename UnaryFunction>
        UnaryFunction for_each(BeginIt first, EndIt last, UnaryFunction f )
        {
        	for (; first != last; ++first) {
                f(*first);
            }
            return f;
        }

        template<typename T>
        static T nextMul(T a, T b)
        {
            return ( (a-1) / b + 1) * b;
        }

        template<typename T>
        std::vector<T> toVector(T t)
        {
            return std::vector<T>(1, t);
        }

        template<typename T>
        std::vector<T> toVector(std::vector<T> const & t)
        {
            return t;
        }

        template<typename T, int NbDim>
        std::vector<T> toVector(T const (&array)[NbDim])
        {
            std::vector<T> v(NbDim);
            for(int i(0); i < NbDim; ++i)
                v[i] = array[i];
            return v;
        }

        template<typename T>
        T const * ptrOf(std::unique_ptr<T> const & data){
            return data.get();
        }

        template<typename T>
        T * ptrOf(std::unique_ptr<T> & data){
            return data.get();
        }

        template<typename T>
        T * ptrOf(std::vector<T> & data){
            return data.data();
        }

        template<typename T>
        T const * ptrOf(std::vector<T> const & data){
            return data.data();
        }

        template<typename T>
        T const * ptrOf(std::initializer_list<T> const & data){
            return data.begin();
        }

        template<typename T>
        T const * ptrOf(T const * data){
            return data;
        }

        template<typename T>
        T * ptrOf(T * data){
            return data;
        }

        template<typename T, typename D>
        T* ptrOf(D & data)
        {
            return data;
        }

        template<typename T, typename D>
        T const * ptrOf(D const & data)
        {
            return data;
        }

        template<typename T>
        SizeT size(std::unique_ptr<T> const & data){
            return SIZE_MAX;
        }

        template<typename T>
        SizeT size(std::vector<T> const & data){
            return data.size();
        }

        template<typename T>
        SizeT size(std::initializer_list<T> const & data){
            return data.size();
        }

        template<typename T>
        SizeT size(T const * data){
            return SIZE_MAX;
        }

        template<typename T>
        SizeT size(T const & data)
        {
            return SIZE_MAX;
        }


        template<typename Ret, typename Data>
        typename std::enable_if<std::is_same<Data, Ret>::value, Ret>::type convert(Data d)
        {
            return d;
        }

        template<typename Ret, typename Data>
        typename std::enable_if<is_iterable<Data>::value, Ret *>::type convert(Data & d)
        {
            return ptrOf<Ret>(d);
        }

        template<typename Ret, typename Data>
        typename std::enable_if<is_iterable<Data>::value, Ret const *>::type convert(Data const & d)
        {
            return ptrOf<Ret>(d);
        }
    }
}

#endif //FUNCTION_H
