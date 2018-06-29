#include <gtest/gtest.h>
#include <vector>

#include <ma/ma>

using namespace std;
using namespace ma;

using namespace ma::shape;
using namespace ma::range;

namespace
{
    MShape<LinearRange> ls({2,3,4});

    TEST(MShapeLinearTest, RightNumberOfDimension)
    {
        EXPECT_EQ(ls.nbDim(), 3);
        EXPECT_EQ(ls.nbActiveDim(), 3);
    }

    TEST(MShapeLinearTest, RightSize)
    {
        EXPECT_EQ(ls.size(), 2 * 3 * 4);
        EXPECT_EQ(ls.size(), ls.baseSize());
    }

    TEST(MShapeLinearTest, Position)
    {
        EXPECT_EQ(ls.at(0), 0);

        EXPECT_EQ(ls.at(13), 13);

        EXPECT_EQ(ls.at(ls.size() - 1), (ls.size() - 1));
    }

    TEST(MShapeLinearTest, RightDimensionAfterClosing)
    {
        auto sub = ls.subShape(1);

        EXPECT_EQ(sub.nbActiveDim(), 2);

        EXPECT_EQ(sub.baseSize(), 2 * 3 * 4);

        EXPECT_EQ(sub.size(), 3 * 4);
    }

    TEST(MShapeLinearTest, SubPosition)
    {
        auto sub = ls.subShape(1);

        EXPECT_EQ(sub.at(0), 12);

        EXPECT_EQ(sub.at(4), 12 + 4);

        EXPECT_EQ(sub.at(sub.size() - 1), (12 + sub.size() - 1));
    }

    TEST(MShapeLinearTest, RightDimensionAfterSelect)
    {
        auto sub = ls.subShape(1, L(1,3), 2);

        EXPECT_EQ(sub.nbActiveDim(), 1);

        EXPECT_EQ(sub.baseSize(), 2 * 3 * 4);

        EXPECT_EQ(sub.size(), 2);
    }

    TEST(MShapeLinearTest, SubPositionComplex)
    {
        auto sub = ls.subShape(1, L(1,3), 2);

        EXPECT_EQ(sub.at(0), 18);

        EXPECT_EQ(sub.at(1), 22);
    }

    MShape<Range> s({2,3,4});

    TEST(MShapeTest, RightNumberOfDimension)
    {
        EXPECT_EQ(s.nbDim(), 3);
        EXPECT_EQ(s.nbActiveDim(), 3);
    }

    TEST(MShapeTest, RightSize)
    {
        EXPECT_EQ(s.size(), 2 * 3 * 4);
        EXPECT_EQ(s.size(), s.baseSize());
    }

    TEST(MShapeTest, RightDimensionAfterClosing)
    {
        auto sub = s.subShape(1);

        EXPECT_EQ(sub.nbActiveDim(), 2);

        EXPECT_EQ(sub.baseSize(), 2 * 3 * 4);

        EXPECT_EQ(sub.size(), 3 * 4);
    }

    TEST(MShapeTest, RightDimensionAfterSelect)
    {
        auto sub = s.subShape(1, vector<int>{2,0}, 2);

        EXPECT_EQ(sub.nbActiveDim(), 1);

        EXPECT_EQ(sub.baseSize(), 2 * 3 * 4);

        EXPECT_EQ(sub.size(), 2);
    }

    TEST(MShapeTest, SubPositionComplex)
    {
        auto sub = s.subShape(1, vector<int>{2,0}, 2);

        EXPECT_EQ(sub.at(0), 22);

        EXPECT_EQ(sub.at(1), 14);
    }
}
