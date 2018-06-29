#include <gtest/gtest.h>
#include <vector>

#include <ma/ma>

using namespace ma::range;
using namespace std;

namespace
{

    Range r1(vector<ma::SizeT>{0,0,1,1,2,2});

    TEST(RangeTest, RangeSize)
    {
        EXPECT_EQ( r1.size(), 6 );
    }

    TEST(RangeTest, RangeProperties)
    {
        EXPECT_FALSE( r1.isComplete(6) );

        EXPECT_TRUE( r1.isActive() );
    }

    Range r2(r1.select(Range(0,6,2)));

    TEST(SubRangeTest, RangeSize)
    {
        EXPECT_EQ( r2.size(), 3 );
    }

    TEST(SubRangeTest, RangeProperties)
    {
        EXPECT_TRUE( r2.isComplete(3) );

        EXPECT_TRUE( r2.isActive() );
    }

}
