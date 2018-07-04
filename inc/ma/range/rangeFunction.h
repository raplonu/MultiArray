#ifndef MA_RANGE_RANGE_FUNCTION_H
#define MA_RANGE_RANGE_FUNCTION_H

// #include <vector>
// #include <algorithm>
// #include <tuple>

#include <ma/range/LinearRange.h>

namespace ma
{
    namespace range
    {
        /**
         * Test if ranges have value
         **/
        constexpr inline bool rangeActive(const LinearRange & lr) noexcept
        {
            return lr.active();
        }

        template<typename T, typename = IsNotLinearRange<T>>
        constexpr bool rangeActive(T const & range) noexcept
        {
            return size(range) != 0;
        }

        /**
         * Test if range have only contiguous
         **/
        constexpr inline bool rangeContiguous(const LinearRange & lr)
        {
            return lr.step() == 1;
        }

        template<typename T, typename = IsNotLinearRange<T>>
        bool rangeContiguous(const T & range)
        {
            for(auto e : LinearRange(size(range))
                if(SizeT(range[e]) != e)
                    return false;

            return true;
        }

        /**
         * Test if ranges have constant step between value
         **/
        constexpr inline bool rangeHasStep(const LinearRange &) noexcept
        {
            return true;
        }

        template<typename T, typename = IsNotLinearRange<T>>
        constexpr bool rangeHasStep(const T & range)
        {
            constexpr auto it(begin(range));
            constexpr DiffT step(*next(it) - *it);
            constexpr SizeT lastE(*it);

            for(++it; it != end(range); ++it)
                if(step != DiffT(*it) - DiffT(lastE))
            		return false;
                else
                	lastE = *it;

            return true;
        }


        // using FullLinearIndice = std::tuple<SizeT, SizeT, DiffT>;
        // using DelayLinearIndice = std::tuple<SizeT, DiffT>;

        // struct FullLinearIndice
        // {
        //     SizeT begin;
        //     SizeT end;
        //     DiffT step;
        // };

        // struct DelayLinearIndice
        // {
        //     SizeT begin;
        //     DiffT step;
        // };

        // inline FullLinearIndice L(SizeT stop)
        // {
        //     return FullLinearIndice{0, stop, 1};
        // }

        // inline FullLinearIndice L(SizeT start, SizeT stop)
        // {
        //     return FullLinearIndice{start, stop, (stop >= start)?1:-1};
        // }

        // inline FullLinearIndice L(SizeT start, SizeT stop, DiffT step)
        // {
        //     return FullLinearIndice{start, stop, step};
        // }

        // inline DelayLinearIndice L(SizeT start, detail::Delay_, DiffT step = 1)
        // {
        //     return DelayLinearIndice{start, step};
        // }

        // template<typename... I>
        // std::vector<SizeT> V(I... i)
        // {
        //     return std::vector<SizeT>{i...};
        // }

        // inline SizeT rangeNbRangedElement(LinearRange const & range)
        // {
        //     return range.nbRangedElement();
        // }

        // template
        // <
        //     typename T,
        //     typename = typename std::enable_if<!std::is_base_of<LinearRange, T>::value>::type
        // >
        // SizeT rangeNbRangedElement(T const & range)
        // {
        //     SizeT rangedE(range.size()), lastRangedE(0), lastVal(range.front());

        //     for_each(++(range.begin()), range.end(),
        //         [&](SizeT pos)
        //         {
        //             if(pos == lastVal + 1)
        //                 ++lastRangedE;
        //             else
        //                 rangedE = std::min(rangedE, lastRangedE);
        //             lastVal = pos;
        //         }
        //     );

        //     return std::max(rangedE, 1ul);
        // }


        // inline bool rangeComplete(LinearRange const & lr, SizeT totalLength)
        // {
        //     return lr.isComplete(totalLength);
        // }

        // template
        // <
        //     typename T,
        //     typename = typename std::enable_if<!std::is_base_of<LinearRange, T>::value>::type
        // >
        // inline bool rangeComplete(T const & range, SizeT length)
        // {
        //     if(range.size() != length)
        //         return false;

        //     return rangeHasSpace(range);
        // }


        // inline DiffT rangeStep(LinearRange const & range)
        // {
        //     return range.step();
        // }

        // template
        // <
        //     typename T,
        //     typename = typename std::enable_if<!std::is_base_of<LinearRange, T>::value>::type
        // >
        // inline DiffT rangeStep(T const & range)
        // {
        //     auto it(range.begin());
        //     return *next(it) - *it;
        // }
    }

    // using range::L;
    // using range::V;
}

#endif //MA_RANGE_RANGE_FUNCTION_H
