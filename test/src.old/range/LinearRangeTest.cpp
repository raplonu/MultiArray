#include <gtest/gtest.h>
#include <ma/ma>

using namespace ma::range;

namespace
{
    LinearRange lr(4);

    TEST(LinearRangeTest, LinearRangeBaseLoop)
    {
        int sum(0);

        for(auto e : lr)
            sum += e;

        EXPECT_EQ( sum, 6 );
        EXPECT_EQ( lr.begin() + 4, lr.end());
    }

    TEST(LinearRangeTest, LinearRangeNegativeStep)
    {
        LinearRange nlr(10, -1);

        EXPECT_EQ( nlr[-10], 0 );

    }
}
