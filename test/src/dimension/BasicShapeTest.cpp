#include <gtest/gtest.h>

#include <ma/dimension/BasicShape.h>

using namespace ma;
using namespace ma::dimension;

namespace
{
    TEST(BasicShape, EmptyBasicShape)
    {
        BasicShape s;

        EXPECT_FALSE(s.active());
        EXPECT_EQ(s.size(), 0);
        EXPECT_EQ(s.baseSize(), 0);

        EXPECT_EQ(s.at(0), 0);
    }

    TEST(BasicShape, SimpleBasicShape)
    {
        BasicShape s(10);

        EXPECT_TRUE(s.active());
        EXPECT_EQ(s.size(), 10);
        EXPECT_EQ(s.baseSize(), 10);

        EXPECT_EQ(s.at(5), 5);
    }
}

