#include <gtest/gtest.h>

#include <ma/iterator/LinearIterator.h>

using namespace ma;
using namespace ma::iterator;

namespace
{
    TEST(LinearIterator, SimpleShift)
    {
        LinearIterator l;

        EXPECT_EQ(*l, 0);
        EXPECT_EQ(*(l + 1), 1);
        l += 2;
        EXPECT_EQ(*l, 2);
    }

    TEST(LinearIterator, CustomStep)
    {
        LinearIterator l(0, 3);

        EXPECT_EQ(*(l + 1), 3);

        EXPECT_EQ(*(l - 1), -3);
    }

    TEST(LinearIterator, CustomStart)
    {
        LinearIterator l(10);

        EXPECT_EQ(*(l + 1), 11);

        l -= 5;

        EXPECT_EQ(*l, 5);
    }
}

