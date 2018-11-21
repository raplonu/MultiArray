#include <gtest/gtest.h>

#include <ma>

using namespace ma;

namespace
{
    TEST(ArrayTest, EmptyBArray)
    {
        BArray<int> a;

        EXPECT_EQ(a.ptr(), nullptr);
        EXPECT_EQ(a.size(), 0);

    }

    TEST(ArrayTest, EmptyMArray)
    {
        MArray<int> a;

        EXPECT_EQ(a.ptr(), nullptr);
        EXPECT_EQ(a.size(), 0);

    }

    TEST(ArrayTest, BSharedArray)
    {
        BSharedArray<int> a;

        EXPECT_EQ(a.ptr(), nullptr);
        EXPECT_EQ(a.size(), 0);

    }

    TEST(ArrayTest, MSharedArray)
    {
        MSharedArray<int> a;

        EXPECT_EQ(a.ptr(), nullptr);
        EXPECT_EQ(a.size(), 0);

    }

    struct TestStruct
    {
        int i;
    };

    TEST(ArrayTest, MSharedArrayWithObject)
    {
        MSharedArray<TestStruct> a;

        EXPECT_EQ(a.ptr(), nullptr);
        EXPECT_EQ(a.size(), 0);

    }

    TEST(ArrayTest, MArray)
    {
        MArray<int> a(10);

        EXPECT_NE(a.ptr(), nullptr);
        EXPECT_EQ(a.size(), 10);
        EXPECT_EQ(a.baseSize(), 10);

    }

    TEST(ArrayTest, GetView)
    {
        MArray<int> a(10);

        auto v = a.at(L(5,delay));

        EXPECT_EQ(a.ptr() + 5, v.ptr());
        EXPECT_EQ(v.size(), a.size() - 5);
    }

    TEST(ArrayTest, ViewFill)
    {
        MArray<int> a(10);

        auto v = a.at(L(5,delay));

        v.setMem(42);

        EXPECT_EQ(a.val(5), 42);
    }

    TEST(ArrayTest, MArrayWithData)
    {
        MArray<int> a(3, {4,5,6});

        EXPECT_EQ(a.size(), 3);

        EXPECT_EQ(a.val(0), 4);
        EXPECT_EQ(a.val(1), 5);
        EXPECT_EQ(a.val(2), 6);
    }

    TEST(ArrayTest, ViewToMArray)
    {
        MArray<int> aa(3, {4,5,6});

        auto v = aa.at(all);

        MArray<int> a(v.shape(), v);

        EXPECT_EQ(a.size(), 3);

        EXPECT_EQ(a.val(0), 4);
        EXPECT_EQ(a.val(1), 5);
        EXPECT_EQ(a.val(2), 6);
    }

    //Fake test, just an andy sandbox for test
    TEST(ArrayTest, MultiArray)
    {
        MArray<int> aa({2,3,4}, 42);

        EXPECT_TRUE(true);

    }

    TEST(ArrayTest, PointerOfPointerTest)
    {
        int i = 42;
        BArray<int*> aa(1, &i);
        int *ptr;
        
        aa.copyTo(&ptr);

        EXPECT_EQ(*aa.ptr(), ptr);
        EXPECT_EQ(&i, ptr);
    }
}

