#include <gtest/gtest.h>
#include <ma/ma>

using namespace std;
using namespace ma;
using namespace ma::container;

namespace
{
    SharedContainer<int> sc{0,1,2,3,4,5,6};

    TEST(SharedContainerTest, CanAccessValue)
    {
        EXPECT_EQ(sc[0], 0);
        EXPECT_EQ(sc[1], 1);
        EXPECT_EQ(sc[2], 2);
        EXPECT_EQ(sc[6], 6);
    }

    TEST(SharedContainerTest, CanChangeValue)
    {
        sc[0] = 666;
        EXPECT_EQ(sc[0], 666);
    }


    TEST(SharedContainerTest, ShareTheData)
    {
        SharedContainer<int> sc1(sc);
        sc[0] = 888;
        EXPECT_EQ(sc1[0], 888);
    }

    TEST(SharedContainerTest, DataIsSyncInToWay)
    {
        SharedContainer<int> sc1(sc);
        sc1[4] = 128;
        EXPECT_EQ(sc[4], 128);
    }

}
