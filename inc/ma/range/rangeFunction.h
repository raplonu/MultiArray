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
        constexpr inline bool rangeContiguousFromZero(const LinearRange & lr)
        {
            return lr.contiguousFromZero();
        }

        template<typename T, typename = IsNotLinearRange<T>>
        bool rangeContiguousFromZero(const T & range)
        {
            for(auto e : LinearRange(size(range)))
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

        namespace impl
        {
            template<typename T>
            struct diff_not_equal_t
            {
                T ref_;

                constexpr explicit diff_not_equal_t(T ref) noexcept: ref_(ref){}

                template<typename TT>
                constexpr bool operator()(const TT & lhs, const TT & rhs) const noexcept
                {
                    return (rhs - lhs) != ref_;
                }
            };

            template<typename T>
            constexpr diff_not_equal_t<T> diff_not_equal(T ref)
            {
                return diff_not_equal_t<T>(std::move(ref));
            }
        }

        template<typename T, typename = IsNotLinearRange<T>>
        MLCONSTEXPR bool rangeHasStep(const T & range)
        {
            auto first = ma::begin(range);
            auto second = ma::next(first);
            auto last = ma::end(range);

            //Range is empty or 1 element ?
            if(empty(range) || second == last) return true;

            auto step = *second - *first;

            return (std::adjacent_find(first, last, impl::diff_not_equal(step)) == last);
        }

        constexpr inline DiffT rangeStep(const LinearRange & range) noexcept
        {
            return range.step();
        }

        template<typename T, typename = IsNotLinearRange<T>>
        MLCONSTEXPR DiffT rangeStep(T const & range)
        {
            auto first = ma::begin(range), second = ma::next(first), last = ma::end(range);

            //Range is empty or 1 element ? maybe return 0 !!!!
            return (first == last || second == last) ? 1 : *second - *first;
        }

        constexpr inline SizeT rangeRangedElementNb(const LinearRange & range) noexcept
        {
            return range.rangedElementNb();
        }

        namespace impl
        {
            struct is_not_contiguous
            {
                template<typename TT>
                constexpr bool operator()(const TT & lhs, const TT & rhs) const noexcept
                {
                    return lhs + 1 != rhs;
                }
            };

        }

        template<typename T, typename = IsNotLinearRange<T>>
        MLCONSTEXPR SizeT rangeRangedElementNb(const T & range)
        {
            auto first = ma::begin(range), step = first, last = ma::end(range);

            SizeT rangedElementNb = size(range);

            while((step = std::adjacent_find(first, last, impl::is_not_contiguous())) != last)
            {
                rangedElementNb = min(rangedElementNb, ma::distance(first, ++step));
                first = step;
            }

            return min(rangedElementNb, ma::distance(first, step));
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



    }

    // using range::L;
    // using range::V;
}

#endif //MA_RANGE_RANGE_FUNCTION_H
