#ifndef MA_DIMENSION_DIMENSION_FUNCTION_H
#define MA_DIMENSION_DIMENSION_FUNCTION_H

#include <ma/detail/base.h>

#include <ma/range/LinearRange.h>
#include <ma/range/rangeFunction.h>

namespace ma
{
    namespace dimension
    {
        namespace impl
        {
            template<typename D>
            D makeDim(D const & dim, range::FullLinearIndice range)
            {
                return dim.select(D(range::LinearRange(std::get<0>(range), std::get<1>(range), std::get<2>(range))));
            }

            template<typename D>
            D makeDim(D const & dim, range::DelayLinearIndice range)
            {
                return dim.select(D(range::LinearRange(std::get<0>(range), dim.size(), std::get<1>(range))));
            }

            template<typename D, typename Size, typename = typename std::enable_if<std::is_integral<Size>::value>::type>
            D makeDim(D const & dim, Size range)
            {
                return dim.closeAt(range);
            }

            template<typename D, typename R, typename = typename std::enable_if<!std::is_integral<R>::value>::type>
            D makeDim(D const & dim, R const & range)
            {
                return dim.select(D(range));
            }

            template<typename D>
            D makeDim(D const & dim, detail::All const &)
            {
                return dim;
            }

            template<typename... R> struct SelectRange;

            template<typename F, typename... R>
            struct SelectRange<F, R...>
            {
                template<typename D>
                static void select
                (
                    D newDimIT, D dimIT, D stopIT,
                    F && fRange, R&&... ranges
                ){
                    //ERROR
                    if(dimIT == stopIT)
                        return;

                    if(dimIT->isActive())
                    {
                        *newDimIT = makeDim(*dimIT, fRange);
                        SelectRange<R...>::select
                        (
                            ++newDimIT, ++dimIT, stopIT,
                            std::forward<R>(ranges)...
                        );
                    }
                    else
                    {
                        *newDimIT = *dimIT;
                        SelectRange<F, R...>::select
                        (
                            ++newDimIT, ++dimIT, stopIT,
                            std::forward<F>(fRange), std::forward<R>(ranges)...
                        );
                    }
                }
            };

            template<>
            struct SelectRange<>
            {
                template<typename D>
                static void select(D newDimIT, D dimIT, D stopIT)
                {
                    while(dimIT != stopIT)
                        *(newDimIT++) = *(dimIT++);
                }
            };
        }

        template<typename D, typename... R>
        std::vector<D> selectDimensions(std::vector<D> dims, R&&... ranges)
        {
            std::vector<D> newDims(dims.size());

            impl::SelectRange<R...>::select
            (
                newDims.begin(), dims.begin(), dims.end(),
                std::forward<R>(ranges)...
            );

            return newDims;
        }





        template
        <
            typename Dim, typename Pos,
            typename = typename std::enable_if<std::is_integral<Pos>::value>::type
        >
        void setDim(Dim & dim, Pos const & s)
        {
            dim = Dim(range::LinearRange(s, s));
        }

        template <typename Dim>
        void setDim(Dim & dim, range::LinearRange const & r)
        {
            dim = Dim(r);
        }

        template<typename Dim, typename... L> struct ResetDimVect;

        template <typename Dim, typename First, typename... L>
        struct ResetDimVect<Dim, First, L...>
        {
            static void todo(Dim * dim, First const & f, L&&... lengths)
            {
                setDim(*dim, f);
                ResetDimVect<Dim, L...>::todo(++dim, std::forward<L>(lengths)...);
            }
        };

        template<typename Dim>
        struct ResetDimVect<Dim>
        {
            static void todo(Dim * dim)
            {}
        };

        template<typename T, typename... L>
        std::vector<T> initDimVectReq(L&&... lengths)
        {
            std::vector<T> res(sizeof...(L));

            ResetDimVect<T, L...>::todo(res.data(), std::forward<L>(lengths)...);

            return res;
        }


        template<typename Dim, typename L>
        void resetDimVect(std::vector<Dim> & dims, L const & lengths)
        {
            std::transform(
                lengths, lengths + dims.size(), dims.begin(),
                [](SizeT length){return Dim(length);}
            );
        }

        template<typename T, typename L>
        std::vector<T> initDimVect(L length) {
            return std::vector<T>(1, length);
        }

        template<typename T, typename L>
        std::vector<T> initDimVect(std::vector<L> const & lengths)
        {
            std::vector<T> res(lengths.size());

            resetDimVect<T>(res, lengths.cbegin());

            return res;
        }

        template<typename T, typename L>
        std::vector<T> initDimVect(std::initializer_list<L> const & lengths)
        {
            std::vector<T> res(lengths.size());

            resetDimVect<T>(res, lengths.begin());

            return res;
        }

    }
}

#endif //MA_DIMENSION_DIMENSION_FUNCTION_H
