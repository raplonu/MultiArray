#include <gtest/gtest.h>
#include <ma/ma>

using namespace ma::iterator;

namespace
{
    LinearIterator li(5);


    TEST(LinearIteratorTest, LinearIteratorBaseAddition)
    {
        EXPECT_EQ( li.value(), 5 );

        ++li;

        EXPECT_EQ( li.value(), 6 );

        li += -2;

        EXPECT_EQ( li.value(), 4 );
    }

    TEST(LinearIteratorTest, LinearIteratorComparaison)
    {
        LinearIterator li1(5), li2(5);

        EXPECT_EQ( li1, li2 );

        li1++;

        EXPECT_TRUE( li1 > li2 );

        EXPECT_TRUE( li1 == ++li2 );
    }
}
