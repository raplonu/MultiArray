#include <gtest/gtest.h>
#include <ma/ma>

using namespace std;
using namespace ma;
using namespace ma::shape;


namespace
{
    SimpleShape ss{100};

    TEST(SimpleShapeTest, GoodSize)
    {
        EXPECT_EQ(ss.size(), 100);
        EXPECT_EQ(ss.baseSize(), 100);

        EXPECT_EQ(ss.at(0), 0);
        EXPECT_EQ(ss.at(99), 99);
    }

    TEST(SimpleShapeTest, MakeSubShape)
    {
        SimpleShape ss1(ss.subShape(10, 90));

        EXPECT_EQ(ss1.size(), 80);
        EXPECT_EQ(ss1.baseSize(), 100);

        EXPECT_EQ(ss1.at(0), 10);
        EXPECT_EQ(ss1.at(79), 89);
    }

    TEST(SimpleShapeTest, CanReset)
    {
        SimpleShape ss1(ss.subShape(10, 90));
        ss1.reset();

        EXPECT_EQ(ss1.size(), 100);
        EXPECT_EQ(ss1.baseSize(), 100);

        EXPECT_EQ(ss1.at(0), 0);
        EXPECT_EQ(ss1.at(99), 99);
    }
}
