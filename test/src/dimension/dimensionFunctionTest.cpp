#include <gtest/gtest.h>

#include <ma/range/LinearRange.h>
#include <ma/range/Range.h>
#include <ma/dimension/dimensionFunction.h>

using namespace ma;
using namespace ma::range;
using namespace ma::dimension;

namespace
{
    TEST(dimensionFunctionTest, OneLinearDimensionInit)
    {
        VectDimension<LinearRange> v = initVectDim<LinearRange>(10);

        EXPECT_FALSE(ma::empty(v));
        EXPECT_EQ(ma::size(v), 1);

        EXPECT_EQ(v[0].size(), 10);
    }

    TEST(dimensionFunctionTest, MultiLinearDimensionInitWithInitList)
    {
        std::initializer_list<int> values = {3,4,5};
        VectDimension<LinearRange> v = initVectDim<LinearRange>(values);

        EXPECT_FALSE(ma::empty(v));
        EXPECT_EQ(ma::size(v), 3);

        EXPECT_EQ(v[0].size(), 3);
        EXPECT_EQ(v[1].size(), 4);
        EXPECT_EQ(v[2].size(), 5);
    }

    TEST(dimensionFunctionTest, MultiLinearDimensionInitWithArray)
    {
        int values[] = {3,4,5};
        VectDimension<LinearRange> v = initVectDim<LinearRange>(values);

        EXPECT_FALSE(ma::empty(v));
        EXPECT_EQ(ma::size(v), 3);

        EXPECT_EQ(v[0].size(), 3);
        EXPECT_EQ(v[1].size(), 4);
        EXPECT_EQ(v[2].size(), 5);
    }

    TEST(dimensionFunctionTest, makeDimFromLinearRange)
    {
        Dimension<LinearRange> dim(10);

        Dimension<LinearRange> d = makeDim(dim, L(10,0,-2));

        EXPECT_TRUE(d.active());
        EXPECT_EQ(d.size(), 5);
        EXPECT_EQ(d.baseSize(), 10);

        EXPECT_EQ(d.begin() + 5, d.end());
        EXPECT_EQ(*d.begin(), 10);
        EXPECT_EQ(*(d.end() - 1), 2);
    }

    TEST(dimensionFunctionTest, makeDimFromValue)
    {
        Dimension<LinearRange> dim(10);

        Dimension<LinearRange> d = makeDim(dim, 5);

        EXPECT_FALSE(d.active());
        EXPECT_EQ(d.size(), 1);
        EXPECT_EQ(d.baseSize(), 10);

        EXPECT_EQ(d.begin(), d.end());
        EXPECT_EQ(*d.begin(), 5);
    }

    TEST(dimensionFunctionTest, makeDimFromDelay)
    {
        Dimension<LinearRange> dim(12);

        Dimension<LinearRange> d = makeDim(dim, L(2, delay, 2));

        EXPECT_TRUE(d.active());
        EXPECT_EQ(d.size(), 5);
        EXPECT_EQ(d.baseSize(), 12);

        EXPECT_EQ(d.begin() + 5, d.end());
        EXPECT_EQ(*d.begin(), 2);
        EXPECT_EQ(*(d.end() - 1), 10);
    }

    TEST(dimensionFunctionTest, makeDimFromVector)
    {
        Dimension<Range> dim(4);

        Dimension<Range> d = makeDim(dim, A(0,2,3,3,1,2));

        EXPECT_TRUE(d.active());
        EXPECT_EQ(d.size(), 6);
        EXPECT_EQ(d.baseSize(), 4);

        EXPECT_EQ(d.begin() + 6, d.end());
        EXPECT_EQ(*d.begin(), 0);
        EXPECT_EQ(*(d.end() - 1), 2);
    }

    TEST(dimensionFunctionTest, selectDimensionsWithValue)
    {
        std::initializer_list<int> values = {5,5,5};
        VectDimension<LinearRange> dims = initVectDim<LinearRange>(values);
        
        VectDimension<LinearRange> d = selectDimensions(dims, 1,2,3);

        EXPECT_EQ(d.size(), 3);

        EXPECT_EQ(d[0].front(), 1);
        EXPECT_EQ(d[1].front(), 2);
        EXPECT_EQ(d[2].front(), 3);

        EXPECT_FALSE(d[0].active());
        EXPECT_FALSE(d[1].active());
        EXPECT_FALSE(d[2].active());
    }

    TEST(dimensionFunctionTest, selectDimensionsWithRanges)
    {
        std::initializer_list<int> values = {5,5,5};
        VectDimension<LinearRange> dims = initVectDim<LinearRange>(values);
        
        VectDimension<LinearRange> d = selectDimensions(dims, L(4,-1,-1), all, 3);

        EXPECT_EQ(d.size(), 3);

        EXPECT_EQ(d[0].front(), 4);
        EXPECT_EQ(d[1].front(), 0);
        EXPECT_EQ(d[2].front(), 3);

        EXPECT_TRUE (d[0].active());
        EXPECT_TRUE (d[1].active());
        EXPECT_FALSE(d[2].active());
    }


}

