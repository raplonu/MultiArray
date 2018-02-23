#include <gtest/gtest.h>
#include <vector>

#include <ma/ma>

using namespace std;
using namespace ma;

namespace
{

    TEST(MemCopyTest, STDContainerMemCopy)
    {
        vector<int> v1 (3,1), v2 (3,0);
        copy::memCopy<int>(v2, v1);

        EXPECT_EQ(v2[0], 1);
        EXPECT_EQ(v2[1], 1);
        EXPECT_EQ(v2[2], 1);
    }

    TEST(MemCopyTest, STDContainerFill)
    {
        vector<int> v (3,0);
        copy::memCopy<int>(v, 42);

        EXPECT_EQ(v[0], 42);
        EXPECT_EQ(v[1], 42);
        EXPECT_EQ(v[2], 42);
    }

    TEST(MemCopyTest, STDPointerMemCopy)
    {
        vector<int> v1 (3,1), v2 (4,0);
        int *p(v2.data());
        copy::memCopy<int>(p, v1);

        EXPECT_EQ(v2[0], 1);
        EXPECT_EQ(v2[1], 1);
        EXPECT_EQ(v2[2], 1);
        EXPECT_EQ(v2[3], 0);
    }

    TEST(MemCopyTest, STDArrayMemCopy)
    {
        vector<int> v1 (3,1);
        SArray<int> sa(4);

        SArray<int> sub = sa.at(0, 3);

        copy::memCopy<int>(sub, v1);

        EXPECT_EQ(sa.value(0), 1);
        EXPECT_EQ(sa.value(1), 1);
        EXPECT_EQ(sa.value(2), 1);
        EXPECT_EQ(sa.value(3), 0);
    }

    TEST(MemCopyTest, STDContainerMemCopyStep)
    {
        vector<int> v1 (3,1), v2 (3,0);
        copy::memCopyStep<int>(v2, v1);

        EXPECT_EQ(v2[0], 1);
        EXPECT_EQ(v2[1], 1);
        EXPECT_EQ(v2[2], 1);
    }

    TEST(MemCopyTest, STDContainerFillStep)
    {
        vector<int> v (3,0);
        copy::memCopyStep<int>(v, 42);

        EXPECT_EQ(v[0], 42);
        EXPECT_EQ(v[1], 42);
        EXPECT_EQ(v[2], 42);
    }

    TEST(MemCopyTest, STDPointerMemCopyStep)
    {
        vector<int> v1 (3,1), v2 (4,0);
        int *p(v2.data());
        copy::memCopyStep<int>(p, v1);

        EXPECT_EQ(v2[0], 1);
        EXPECT_EQ(v2[1], 1);
        EXPECT_EQ(v2[2], 1);
        EXPECT_EQ(v2[3], 0);
    }

    TEST(MemCopyTest, STDArrayMemCopyStep)
    {
        vector<int> v1 (3,1);
        MArray<int> sa(6);

        MArray<int> sub = sa.at(L(0, 6, 2));

        copy::memCopyStep<int>(sub, v1);

        EXPECT_EQ(sa.value(0), 1);
        EXPECT_EQ(sa.value(1), 0);
        EXPECT_EQ(sa.value(2), 1);
        EXPECT_EQ(sa.value(3), 0);
        EXPECT_EQ(sa.value(4), 1);
        EXPECT_EQ(sa.value(5), 0);
    }

    TEST(MemCopyTest, MultiDimSTDArrayMemCopyStep)
    {
        vector<int> v1 (4,1);
        MArray<int> sa({4,4}, 0);

        MArray<int> sub = sa.at(L(1, 3), L(1, 3));

        copy::memCopyStep<int>(sub, v1);

        EXPECT_EQ(sa.value(0), 0);
        EXPECT_EQ(sa.value(1), 0);
        EXPECT_EQ(sa.value(2), 0);
        EXPECT_EQ(sa.value(3), 0);
        EXPECT_EQ(sa.value(4), 0);
        EXPECT_EQ(sa.value(5), 1);
        EXPECT_EQ(sa.value(6), 1);
        EXPECT_EQ(sa.value(7), 0);
        EXPECT_EQ(sa.value(8), 0);
        EXPECT_EQ(sa.value(9), 1);
        EXPECT_EQ(sa.value(10), 1);
        EXPECT_EQ(sa.value(11), 0);
        EXPECT_EQ(sa.value(12), 0);
        EXPECT_EQ(sa.value(13), 0);
        EXPECT_EQ(sa.value(14), 0);
        EXPECT_EQ(sa.value(15), 0);
    }
}
