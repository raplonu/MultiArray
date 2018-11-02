#ifndef MA_DIMENSION_DIMENSION_FUNCTION_H
#define MA_DIMENSION_DIMENSION_FUNCTION_H

#include <ma_api/type.h>
#include <ma_api/traits.h>

#include <ma_api/range/LinearRange.h>
#include <ma_api/range/Range.h>

#include <ma_api/dimension/Dimension.h>

namespace ma
{
    namespace dimension
    {
        template<typename Range>
        using VectDimension = std::vector<Dimension<Range>>;

        template<typename Range>
        using DimItIn = typename VectDimension<Range>::iterator;

        template<typename Range>
        using DimItOut = typename VectDimension<Range>::const_iterator;

        /**
         * VectDimension initialization
         **/
        template<typename Range, typename L, typename = IsIntegral<L>>
        VectDimension<Range> initVectDim(L length) {
            return VectDimension<Range>(1, length);
        }

        template<typename Range, typename L, typename = IsIterable<L>>
        VectDimension<Range> initVectDim(const L & lengths)
        {
            return VectDimension<Range>(ma::begin(lengths), ma::end(lengths));
        }

        template<typename Range, typename L, std::size_t N >
        VectDimension<Range> initVectDim(const L (&lengths)[N])
        {
            return VectDimension<Range>(ma::begin(lengths), ma::end(lengths));
        }

        /**
         * Sub Dimension initialization
         **/
        template<typename Range>
        Dimension<Range> makeDim(const Dimension<Range> & dim, const range::LinearRange & range)
        {
            return dim.select(range);
        }

        template<typename Range>
        Dimension<Range> makeDim(const Dimension<Range> & dim, const range::DelayLinearIndice & range)
        {
            return dim.select(range::LinearRange(range.begin, dim.back(), range.step));
        }

        template<typename Range>//, typename Size, typename = typename std::enable_if<std::is_integral<Size>::value>::type>
        Dimension<Range> makeDim(const Dimension<Range> & dim, SizeT pos)
        {
            return dim.closeAt(pos);
        }

        template<typename ORange>
        Dimension<range::Range> makeDim(const Dimension<range::Range> & dim, ORange && range)
        {
            return dim.select(range::Range(forward<ORange>(range)));
        }

        template<typename Range>
        Dimension<Range> makeDim(const Dimension<Range> & dim, All)
        {
            return dim;
        }

        namespace impl
        {
            template<typename Range, typename... R> struct SelectRange;

            template<typename Range, typename Head, typename... Tail>
            struct SelectRange<Range, Head, Tail...>
            {
                static void select
                (
                    DimItIn<Range> out, DimItOut<Range> first, DimItOut<Range> last,
                    Head && head, Tail&&... tail
                ) noexcept {
                    // ERROR
                    massert(first != last);

                    if(first->active())
                    {
                        *out = makeDim(*first, head);
                        SelectRange<Range, Tail...>::select
                        (
                            ++out, ++first, last,
                            forward<Tail>(tail)...
                        );
                    }
                    else
                    {
                        *out = *first;
                        SelectRange<Range, Head, Tail...>::select
                        (
                            ++out, ++first, last,
                            forward<Head>(head), forward<Tail>(tail)...
                        );
                    }
                }
            };

            template<typename Range>
            struct SelectRange<Range>
            {
                static void select(DimItIn<Range> out, DimItOut<Range> first, DimItOut<Range> last) noexcept
                {
                    ma::copy(first, last, out);
                }
            };
        }

        template<typename Range, typename... R>
        VectDimension<Range> selectDimensions(const VectDimension<Range> & dims, R&&... ranges)
        {
            VectDimension<Range> nDims(dims.size());

            impl::SelectRange<Range, R...>::select
            (
                ma::begin(nDims), ma::begin(dims), ma::end(dims),
                forward<R>(ranges)...
            );

            return nDims;
        }





        // template
        // <
        //     typename Dim, typename Pos,
        //     typename = typename std::enable_if<std::is_integral<Pos>::value>::type
        // >
        // void setDim(Dim & dim, Pos const & s)
        // {
        //     dim = Dim(range::LinearRange(s, s));
        // }

        // template <typename Dim>
        // void setDim(Dim & dim, range::LinearRange const & r)
        // {
        //     dim = Dim(r);
        // }

        // template<typename Dim, typename... L> struct ResetDimVect;

        // template <typename Dim, typename First, typename... L>
        // struct ResetDimVect<Dim, First, L...>
        // {
        //     static void todo(Dim * dim, First const & f, L&&... lengths)
        //     {
        //         setDim(*dim, f);
        //         ResetDimVect<Dim, L...>::todo(++dim, std::forward<L>(lengths)...);
        //     }
        // };

        // template<typename Dim>
        // struct ResetDimVect<Dim>
        // {
        //     static void todo(Dim *)
        //     {}
        // };

        // template<typename T, typename... L>
        // std::vector<T> initDimVectReq(L&&... lengths)
        // {
        //     std::vector<T> res(sizeof...(L));

        //     ResetDimVect<T, L...>::todo(res.data(), std::forward<L>(lengths)...);

        //     return res;
        // }



    }
}

#endif //MA_DIMENSION_DIMENSION_FUNCTION_H
