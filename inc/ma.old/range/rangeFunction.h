#ifndef RANGE_FUNCTION_H
#define RANGE_FUNCTION_H

#include <vector>
#include <algorithm>
// #include <tuple>

#include <ma/range/LinearRange.h>

namespace ma
{
    namespace range
    {
        // using FullLinearIndice = std::tuple<SizeT, SizeT, DiffT>;
        // using DelayLinearIndice = std::tuple<SizeT, DiffT>;

        struct FullLinearIndice
        {
            SizeT begin;
            SizeT end;
            DiffT step;
        };

        struct DelayLinearIndice
        {
            SizeT begin;
            DiffT step;
        };

        inline FullLinearIndice L(SizeT stop)
        {
            return FullLinearIndice{0, stop, 1};
        }

        inline FullLinearIndice L(SizeT start, SizeT stop)
        {
            return FullLinearIndice{start, stop, (stop >= start)?1:-1};
        }

        inline FullLinearIndice L(SizeT start, SizeT stop, DiffT step)
        {
            return FullLinearIndice{start, stop, step};
        }

        inline DelayLinearIndice L(SizeT start, detail::Delay_, DiffT step = 1)
        {
            return DelayLinearIndice{start, step};
        }

        template<typename... I>
        std::vector<SizeT> V(I... i)
        {
            return std::vector<SizeT>{i...};
        }

        inline SizeT rangeNbRangedElement(LinearRange const & range)
        {
            return range.nbRangedElement();
        }

        template
        <
            typename T,
            typename = typename std::enable_if<!std::is_base_of<LinearRange, T>::value>::type
        >
        SizeT rangeNbRangedElement(T const & range)
        {
            SizeT rangedE(range.size()), lastRangedE(0), lastVal(range.front());

            for_each(++(range.begin()), range.end(),
                [&](SizeT pos)
                {
                    if(pos == lastVal + 1)
                        ++lastRangedE;
                    else
                        rangedE = std::min(rangedE, lastRangedE);
                    lastVal = pos;
                }
            );

            return std::max(rangedE, 1ul);
        }

        inline bool rangeActive(LinearRange const & lr)
        {
            return lr.isActive();
        }

        template
        <
            typename T,
            typename = typename std::enable_if<!std::is_base_of<LinearRange, T>::value>::type
        >
        inline bool rangeActive(T const & range)
        {
            return range.size() != 0;
        }

        inline bool rangeHasSpace(LinearRange const & range)
        {
            return range.step() != 1;
        }

        template
        <
            typename T,
            typename = typename std::enable_if<!std::is_base_of<LinearRange, T>::value>::type
        >
        bool rangeHasSpace(T const & range)
        {
            for(auto e : LinearRange(range.size()))
                if(SizeT(range[e]) != e)
                    return false;

            return true;
        }

        inline bool rangeComplete(LinearRange const & lr, SizeT totalLength)
        {
            return lr.isComplete(totalLength);
        }

        template
        <
            typename T,
            typename = typename std::enable_if<!std::is_base_of<LinearRange, T>::value>::type
        >
        inline bool rangeComplete(T const & range, SizeT length)
        {
            if(range.size() != length)
                return false;

            return rangeHasSpace(range);
        }

        inline bool rangeHasStep(LinearRange const & range)
        {
            (void)range;
            return true;
        }

        template
        <
            typename T,
            typename = typename std::enable_if<!std::is_base_of<LinearRange, T>::value>::type
        >
        inline bool rangeHasStep(T const & range)
        {
            auto it(range.begin());
            DiffT step(*next(it) - *it);
            SizeT lastE(*it);

            for(++it; it != range.end(); ++it)
                if(step != DiffT(*it) - DiffT(lastE))
            		return false;
                else
                	lastE = *it;

            return true;
        }

        inline DiffT rangeStep(LinearRange const & range)
        {
            return range.step();
        }

        template
        <
            typename T,
            typename = typename std::enable_if<!std::is_base_of<LinearRange, T>::value>::type
        >
        inline DiffT rangeStep(T const & range)
        {
            auto it(range.begin());
            return *next(it) - *it;
        }
    }

    using range::L;
    using range::V;
}

#endif //RANGE_FUNCTION_H
