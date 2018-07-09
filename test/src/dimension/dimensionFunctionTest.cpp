#include <gtest/gtest.h>

#include <ma/range/LinearRange.h>
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

    TEST(dimensionFunctionTest, MultiLinearDimensionInit)
    {
        std::initializer_list<int> values = {3,4,5};
        VectDimension<LinearRange> v = initVectDim<LinearRange>(values);

        EXPECT_FALSE(ma::empty(v));
        EXPECT_EQ(ma::size(v), 3);

        EXPECT_EQ(v[0].size(), 3);
        EXPECT_EQ(v[1].size(), 4);
        EXPECT_EQ(v[2].size(), 5);
    }
}

