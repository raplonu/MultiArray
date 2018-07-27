#include <gtest/gtest.h>

#include <ma_api/range/LinearRange.h>

using namespace ma;
using namespace ma::range;

namespace
{
    TEST(LinearRangeTest, EmptySimpleRange)
    {
        LinearRange r;

        EXPECT_EQ(r.begin(), r.end());
        EXPECT_FALSE(r.active());
        EXPECT_TRUE(r.hasStep());
        EXPECT_EQ(r.size(), 0);
        EXPECT_TRUE(r.complete(0));
    }

    TEST(LinearRangeTest, SimpleRange)
    {
        LinearRange r(10);

        EXPECT_EQ(r.begin() + 10, r.end());
        EXPECT_TRUE(r.active());
        EXPECT_TRUE(r.hasStep());
        EXPECT_EQ(r.size(), 10);
        EXPECT_TRUE(r.complete(10));
    }

    TEST(LinearRangeTest, SimpleRangeWithStep)
    {
        LinearRange r(0, 10, 2);

        EXPECT_EQ(r.begin() + 5, r.end());
        EXPECT_TRUE(r.active());
        EXPECT_TRUE(r.hasStep());
        EXPECT_EQ(r.size(), 5);
        EXPECT_FALSE(r.complete(5));
    }

    TEST(LinearRangeTest, ReverseSimpleRange)
    {
        LinearRange r(10, 0);

        EXPECT_EQ(r.begin() + 10, r.end());

        EXPECT_EQ(r.size(), 10);
        EXPECT_EQ(r.step(), -1);
        EXPECT_FALSE(r.complete(10));
    }

    TEST(LinearRangeTest, ReverseSimpleRangeWithStep)
    {
        LinearRange r(5, -5, -3);

        EXPECT_EQ(r.begin() + 3, r.end());

        EXPECT_EQ(r.size(), 3);
        // Stop is change from -5 to -4 to be equal to start + step * N with N interger
        EXPECT_EQ(r.stop(), -4);
        EXPECT_FALSE(r.complete(3));
    }


}

