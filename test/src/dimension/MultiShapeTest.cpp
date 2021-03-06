#include <gtest/gtest.h>

#include <ma_api/range/LinearRange.h>
#include <ma_api/dimension/MultiShape.h>

using namespace ma;
using namespace ma::range;
using namespace ma::dimension;

namespace
{
    TEST(MultiShapeTest, EmptyMultiShape)
    {
        MultiShape<LinearRange> s;

        EXPECT_TRUE(s.contiguous());
        EXPECT_EQ(s.size(), 0);
        EXPECT_EQ(s.baseSize(), 0);

    }

    TEST(MultiShapeTest, SimpleMultiShape)
    {
        MultiShape<LinearRange> s(10);

        EXPECT_TRUE(s.contiguous());
        EXPECT_EQ(s.size(), 10);
        EXPECT_EQ(s.baseSize(), 10);

        EXPECT_EQ(s.at(5), 5);
    }
}

