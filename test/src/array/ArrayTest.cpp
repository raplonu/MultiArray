#include <gtest/gtest.h>
#include <vector>

#include <ma/ma>

using namespace std;
using namespace ma;

namespace
{
    TEST(ArrayTest, STDContainerMemCopy)
    {
        SArray<int> sa(5, {1,2,3,4,5});
        MArray<int> ma({2,2}, sa.at(1, Delay));

        EXPECT_EQ(sa[0], 1);
        EXPECT_EQ(sa[1], 2);
        EXPECT_EQ(sa[2], 3);
        EXPECT_EQ(sa[3], 4);
        EXPECT_EQ(sa[4], 5);

        EXPECT_EQ(sa.value(0), 1);
        EXPECT_EQ(sa.value(1), 2);
        EXPECT_EQ(sa.value(2), 3);
        EXPECT_EQ(sa.value(3), 4);
        EXPECT_EQ(sa.value(4), 5);

        EXPECT_EQ(sa[0].value(), 1);
        EXPECT_EQ(sa[1].value(), 2);
        EXPECT_EQ(sa[2].value(), 3);
        EXPECT_EQ(sa[3].value(), 4);
        EXPECT_EQ(sa[4].value(), 5);

        EXPECT_EQ(ma.value(0), 2);
        EXPECT_EQ(ma.value(1), 3);
        EXPECT_EQ(ma.value(2), 4);
        EXPECT_EQ(ma.value(3), 5);

        EXPECT_EQ(ma[0][0].value(), 2);
        EXPECT_EQ(ma[0][1].value(), 3);
        EXPECT_EQ(ma[1][0].value(), 4);
        EXPECT_EQ(ma[1][1].value(), 5);

        EXPECT_EQ(ma[0][0], 2);
        EXPECT_EQ(ma[0][1], 3);
        EXPECT_EQ(ma[1][0], 4);
        EXPECT_EQ(ma[1][1], 5);

        ma[0][1] = 42;

        EXPECT_EQ(ma[0][1], 42);
        EXPECT_EQ(sa[2], 42);

    }

    TEST(ArrayTest, SimpleLoopTest)
    {
        int tab[] = {0, 3, 5, 1};
        SArray<int> aa(4, &(tab[0]));

        int pos(0);
        for(auto val : aa)
            EXPECT_EQ(val, tab[pos++]);
    }

    struct Toto{};

    TEST(ArrayTest, ArrayOfStruct)
    {

        Toto t;

        SArray<Toto> sa(5, t);
        sa.setMem(Toto{});
        sa.copyTo(&t);
    }
}