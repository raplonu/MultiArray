#include <gtest/gtest.h>

#include <ma/dimension/Dimension.h>
#include <ma/range/LinearRange.h>

using namespace ma;
using namespace ma::range;
using namespace ma::dimension;

namespace
{
    TEST(DimensionTest, EmptyDimension)
    {
        Dimension<LinearRange> d;

        EXPECT_FALSE(d.active());
        EXPECT_EQ(d.size(), 1);
        EXPECT_EQ(d.baseSize(), 1);

        EXPECT_EQ(d.begin() + 1, d.end());
        EXPECT_EQ(*(d.begin() + 1), *d.end());
    }

    TEST(DimensionTest, SimpleDimension)
    {
        Dimension<LinearRange> d(10);

        EXPECT_TRUE(d.active());
        EXPECT_EQ(d.size(), 10);
        EXPECT_EQ(d.baseSize(), 10);

        EXPECT_EQ(d.begin() + 10, d.end());
        EXPECT_EQ(*d.begin(), 0);
        EXPECT_EQ(*d.end(), 10);
    }

    TEST(DimensionTest, closeDimension)
    {
        Dimension<LinearRange> dd(10);

        auto d = dd.closeAt(5);

        EXPECT_FALSE(d.active());
        EXPECT_EQ(d.size(), 1);
        EXPECT_EQ(d.baseSize(), 10);

        EXPECT_EQ(d.begin() + 1, d.end());
        EXPECT_EQ(*d.begin(), 5);
        EXPECT_EQ(*(d.begin() + 1), *d.end());
    }
    
    TEST(DimensionTest, resetDimension)
    {
        Dimension<LinearRange> dd(10);

        auto d = dd.closeAt(5);
        d.reset();

        EXPECT_TRUE(d.active());
        EXPECT_EQ(d.size(), 10);
        EXPECT_EQ(d.baseSize(), 10);

        EXPECT_EQ(d.begin() + 10, d.end());
        EXPECT_EQ(*d.begin(), 0);
        EXPECT_EQ(*d.end(), 10);
    }

    TEST(DimensionTest, selectSubDimension)
    {
        Dimension<LinearRange> dd(10);

        LinearRange l(0,10,2);

        auto d = dd.select(l);

        EXPECT_TRUE(d.active());
        EXPECT_EQ(d.size(), 5);
        EXPECT_EQ(d.baseSize(), 10);

        EXPECT_EQ(d.begin() + 5, d.end());
        EXPECT_EQ(*d.begin(), 0);
        EXPECT_EQ(*d.end(), 10);
    }
}

