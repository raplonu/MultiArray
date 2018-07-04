#include <gtest/gtest.h>

#include <vector>
#include <deque>

#include <ma/range/rangeFunction.h>

using namespace ma;
using namespace ma::range;

namespace
{
    TEST(rangeFunctionTest, EmptyLinearRangeTest)
    {
        LinearRange r;

        EXPECT_FALSE(rangeActive(r));
        EXPECT_TRUE(rangeContiguousFromZero(r));
        EXPECT_TRUE(rangeHasStep(r));
        EXPECT_EQ(rangeStep(r), 1);
        EXPECT_EQ(rangeRangedElementNb(r), 1);
    }

    TEST(rangeFunctionTest, SimpleLinearRangeTest)
    {
        LinearRange r(10);

        EXPECT_TRUE(rangeActive(r));
        EXPECT_TRUE(rangeContiguousFromZero(r));
        EXPECT_TRUE(rangeHasStep(r));
        EXPECT_EQ(rangeStep(r), 1);
        EXPECT_EQ(rangeRangedElementNb(r), 10);
    }

    TEST(rangeFunctionTest, ComplexLinearRangeTest)
    {
        LinearRange r(0,10,2);

        EXPECT_TRUE(rangeActive(r));
        EXPECT_FALSE(rangeContiguousFromZero(r));
        EXPECT_TRUE(rangeHasStep(r));
        EXPECT_EQ(rangeStep(r), 2);
        EXPECT_EQ(rangeRangedElementNb(r), 1);
    }

    TEST(rangeFunctionTest, EmptyVectorTest)
    {
        std::vector<int> r;

        EXPECT_FALSE(rangeActive(r));
        EXPECT_TRUE(rangeContiguousFromZero(r));
        EXPECT_TRUE(rangeHasStep(r));
        EXPECT_EQ(rangeStep(r), 1);
        EXPECT_EQ(rangeRangedElementNb(r), 0);
    }

    TEST(rangeFunctionTest, SimpleVectorTest)
    {
        std::vector<int> r{0, 1, 2, 3, 4};

        EXPECT_TRUE(rangeActive(r));
        EXPECT_TRUE(rangeContiguousFromZero(r));
        EXPECT_TRUE(rangeHasStep(r));
        EXPECT_EQ(rangeStep(r), 1);
        EXPECT_EQ(rangeRangedElementNb(r), 5);
    }

    TEST(rangeFunctionTest, SimpleVectorTestWithStep)
    {
        std::vector<int> r{0, 2, 4, 6};

        EXPECT_TRUE(rangeActive(r));
        EXPECT_FALSE(rangeContiguousFromZero(r));
        EXPECT_TRUE(rangeHasStep(r));
        EXPECT_EQ(rangeStep(r), 2);
        EXPECT_EQ(rangeRangedElementNb(r), 1);
    }

    TEST(rangeFunctionTest, ComplexVectorTest)
    {
        std::vector<int> r{0, 1, 4, 5, 0, 1};

        EXPECT_TRUE(rangeActive(r));
        EXPECT_FALSE(rangeContiguousFromZero(r));
        EXPECT_FALSE(rangeHasStep(r));
        EXPECT_EQ(rangeStep(r), 1); //But mean nothing
        EXPECT_EQ(rangeRangedElementNb(r), 2);
    }


}

