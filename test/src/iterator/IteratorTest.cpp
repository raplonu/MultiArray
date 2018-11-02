#include <gtest/gtest.h>

#include <ma_api/iterator/Iterator.h>

using namespace ma;
using namespace ma::iterator;

namespace
{
    TEST(Iterator, LinearShift)
    {
        Iterator l;

        EXPECT_EQ(*l, 0);
        EXPECT_EQ(*(l + 1), 1);
        l += 2;
        EXPECT_EQ(*l, 2);
    }

    TEST(Iterator, LinearStep)
    {
        Iterator l(0, 3);

        EXPECT_EQ(*(l + 1), 3);

        EXPECT_EQ(*(l - 1), -3);
    }

    TEST(Iterator, LinearStart)
    {
        Iterator l(10);

        EXPECT_EQ(*(l + 1), 11);

        l -= 5;

        EXPECT_EQ(*l, 5);
    }

    TEST(Iterator, CustomShift)
    {
        std::vector<int> v{4,8,-2};
        Iterator l(v.begin());

        EXPECT_EQ(*l, 4);
        EXPECT_EQ(*(l + 1), 8);
        l += 2;
        EXPECT_EQ(*l, -2);
    }

    TEST(Iterator, CustomCompare)
    {
        std::vector<int> v{4,4};
        Iterator it1(v.begin());
        Iterator it2(it1 + 1);

        EXPECT_NE(it1, it2);
        it1++;
        EXPECT_EQ(it1, it2);
    }

    // TEST(Iterator, NegativIterator)
    // {
    //     Iterator it;

    //     EXPECT_EQ(*it, 0);
    //     --it;
    //     EXPECT_EQ(*it, -1);
    //     ++it;
    //     EXPECT_EQ(*it, 0);
    // }
}

