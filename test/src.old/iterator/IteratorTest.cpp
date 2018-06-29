#include <gtest/gtest.h>
#include <ma/ma>

using namespace ma::iterator;

namespace
{



    TEST(IteratorTest, LinearIteratorBaseAddition)
    {
        Iterator li(5);

        EXPECT_EQ( *li, 5 );

        ++li;

        EXPECT_EQ( *li, 6 );

        li += -2;

        EXPECT_EQ( *li, 4 );
    }

    TEST(IteratorTest, IteratorComparaison)
    {
        Iterator li1(5), li2(5);

        EXPECT_TRUE( li1 == li2 );

        li1++;

        EXPECT_TRUE( li1 != li2 );

        EXPECT_TRUE( li1 == ++li2 );
    }

    TEST(IteratorTest, IteratorStep)
    {
        Iterator li1(5, -1);

        ++li1;

        EXPECT_EQ( *li1, 4 );

        li1 += -2;

        EXPECT_EQ( *li1, 6 );

    }
}
