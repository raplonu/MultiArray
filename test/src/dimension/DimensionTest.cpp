#include <gtest/gtest.h>
#include <vector>

#include <ma/ma>

using namespace ma::dimension;
using namespace ma::range;
using namespace std;

namespace LinearDimensionTest
{
    Dimension<LinearRange> d1(5);

    TEST(LinearDimensionTest, DimensionSize1)
    {
        EXPECT_EQ( d1.size(), 5 );

        EXPECT_EQ( d1.baseSize(), 5 );
    }

    TEST(LinearDimensionTest, DimensionProperties1)
    {
        EXPECT_TRUE( d1.isComplete() );

        EXPECT_TRUE( d1.isActive() );
    }

    Dimension<LinearRange> d2(d1.select(Dimension<LinearRange>(3)));

    TEST(LinearDimensionTest, DimensionSize2)
    {
        EXPECT_EQ( d2.size(), 3 );

        EXPECT_EQ( d2.baseSize(), 5 );
    }

    TEST(LinearDimensionTest, DimensionProperties2)
    {
        EXPECT_FALSE( d2.isComplete() );

        EXPECT_TRUE( d2.isActive() );
    }

    Dimension<LinearRange> d3(d2.closeAt(3));

    TEST(LinearDimensionTest, DimensionSize3)
    {
        EXPECT_EQ( d3.size(), 0 );

        EXPECT_EQ( d3.baseSize(), 5 );
    }

    TEST(LinearimensionTest, DimensionProperties3)
    {
        EXPECT_FALSE( d3.isComplete() );

        EXPECT_FALSE( d3.isActive() );
    }

    TEST(LinearDimensionTest, DimensionPos3)
    {
        EXPECT_EQ( *(d3.begin()), 3 );

    }
}

namespace DimensionTest
{
    Dimension<Range> d1(Range(2,7));

    TEST(DimensionTest, DimensionSize1)
    {
        EXPECT_EQ( d1.size(), 5 );

        EXPECT_EQ( d1.baseSize(), 5 );
    }

    TEST(DimensionTest, DimensionProperties1) 
    {
        EXPECT_FALSE( d1.isComplete() );

        EXPECT_TRUE( d1.isActive() );
    }

    Dimension<Range> dTmp(Range(std::vector<ma::SizeT>{3,2,1}));

    Dimension<Range> d2(d1.select(dTmp));

    TEST(DimensionTest, DimensionSize2)
    {
        EXPECT_EQ( d2.size(), 3 );

        EXPECT_EQ( d2.baseSize(), 5 );
    }

    TEST(DimensionTest, DimensionProperties2)
    {
        EXPECT_FALSE( d2.isComplete() );

        EXPECT_TRUE( d2.isActive() );
    }

    Dimension<Range> d3(d2.closeAt(1));

    TEST(DimensionTest, DimensionSize3)
    {
        EXPECT_EQ( d3.size(), 0 );

        EXPECT_EQ( d3.baseSize(), 5 );
    }

    TEST(DimensionTest, DimensionProperties3)
    {
        EXPECT_FALSE( d3.isComplete() );

        EXPECT_FALSE( d3.isActive() );
    }

    TEST(DimensionTest, DimensionPos3)
    {
        EXPECT_EQ( *(d3.begin()), 4 );

    }

    TEST(DimensionTest, DimensionIt3)
    {
        EXPECT_TRUE( (++d3.begin()) == d3.end() );
    }

    Dimension<Range> d4(Range(std::vector<ma::SizeT>{1}));

    TEST(DimensionTest, DimensionIt4)
    {
        EXPECT_TRUE( (++d4.begin()) == d4.end() );
    }
}
