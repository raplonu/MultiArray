#include <gtest/gtest.h>

#include <ma/range/Range.h>

using namespace ma;
using namespace ma::range;

namespace
{
    TEST(RangeTest, EmptySimpleRange)
    {
        Range r;

        EXPECT_EQ(r.begin(), r.end());
        EXPECT_FALSE(r.active());
        EXPECT_EQ(r.size(), 0);
        EXPECT_TRUE(r.complete(0));
    }

    TEST(RangeTest, SimpleRange)
    {
        Range r(10);

        EXPECT_EQ(r.begin() + 10, r.end());
        EXPECT_TRUE(r.active());
        EXPECT_EQ(r.size(), 10);
        EXPECT_TRUE(r.complete(10));
    }

    TEST(RangeTest, SimpleRangeWithStep)
    {
        Range r(0, 10, 2);

        EXPECT_EQ(r.begin() + 5, r.end());
        EXPECT_TRUE(r.active());
        EXPECT_EQ(r.size(), 5);
        EXPECT_FALSE(r.complete(5));
    }

    TEST(RangeTest, ReverseSimpleRange)
    {
        Range r(10, 0);

        EXPECT_EQ(r.begin() + 10, r.end());

        EXPECT_EQ(r.size(), 10);
        EXPECT_FALSE(r.complete(10));
    }

    TEST(RangeTest, ReverseSimpleRangeWithStep)
    {
        Range r(5, -5, -3);

        EXPECT_EQ(r.begin() + 3, r.end());

        EXPECT_EQ(r.size(), 3);
        // Stop is change from -5 to -4 to be equal to start + step * N with N interger
        EXPECT_EQ(r.stop(), -4);
        EXPECT_FALSE(r.complete(3));
    }


}

