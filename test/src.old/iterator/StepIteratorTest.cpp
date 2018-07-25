#include <gtest/gtest.h>
#include <ma/ma>

#include <vector>

using namespace std;
using namespace ma;

namespace
{

    // TEST( "simple it with std comtainer" )
    // {
    //     vector<int> v1{1,2,3,4};
    //
    //     auto it(ma::iterator::stepIterator<int>(v1, 1));
    //
    //     EXPECT_EQ(*it, 1);
    //     ++it;
    //     EXPECT_EQ(*it, 2);
    //     ++it;
    //     EXPECT_EQ(*it, 3);
    //     ++it;
    //     EXPECT_EQ(*it, 4);
    // }
    //
    // TEST( "step it with std comtainer" )
    // {
    //     vector<int> v1{1,2,3,4,5,6};
    //
    //     auto it(ma::iterator::stepIterator<int>(v1, 2));
    //
    //     EXPECT_EQ(*it, 1);
    //     ++it;
    //     EXPECT_EQ(*it, 3);
    //     ++it;
    //     EXPECT_EQ(*it, 5);
    // }

    TEST(StepIteratorTest, ConstItWithValue)
    {
        auto it(ma::iterator::stepIterator<const int>(int(8), 1));

        EXPECT_EQ(detail::convert<const int>(it), 8);
        ++it;
        EXPECT_EQ(detail::convert<const int>(it), 8);
        ++it;
        EXPECT_EQ(detail::convert<const int>(it), 8);
        ++it;
        EXPECT_EQ(detail::convert<const int>(it), 8);
    }

}
