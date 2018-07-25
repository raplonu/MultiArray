#include <gtest/gtest.h>

#include <ma/array/ArrayView.h>
#include <ma/dimension/BasicShape.h>
#include <ma/dimension/MultiShape.h>
#include <ma/range/LinearRange.h>
#include <ma/function.h>

using namespace ma;
using namespace ma::array;
using namespace ma::dimension;
using namespace ma::range;

namespace
{
    TEST(ArrayViewTest, BasicArrayView)
    {
        ArrayView<int, DefaultAlloc<int>, BasicShape> a;

        EXPECT_EQ(a.ptr(), nullptr);
        EXPECT_EQ(a.size(), 0);

    }

    TEST(ArrayViewTest, MultiArrayView)
    {
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a;

        EXPECT_EQ(a.ptr(), nullptr);
        EXPECT_EQ(a.size(), 0);
    }

    TEST(ArrayViewTest, MultiArrayView1DimFromVector)
    {
        std::vector<int> v(10);
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a(10, v.data());

        EXPECT_EQ(a.ptr(), v.data());
        EXPECT_EQ(a.size(), 10);
    }

    TEST(ArrayViewTest, MultiArrayView1DimFromArray)
    {
        int d[10];
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a(10, d);

        EXPECT_EQ(a.ptr(), d);
        EXPECT_EQ(a.size(), 10);
    }

    TEST(ArrayViewTest, MultiArrayView2Dim)
    {
        std::vector<int> v(10);
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a({2,5}, v.data());

        EXPECT_EQ(a.ptr(), v.data());
        EXPECT_EQ(a.size(), 10);
        
        EXPECT_EQ(a[0].ptr(), v.data());
        EXPECT_EQ(a[0].size(), 5);

        EXPECT_EQ(a[1].ptr(), v.data() + 5);
        EXPECT_EQ(a[1].size(), 5);
    }

    TEST(ArrayViewTest, FillTest)
    {
        std::vector<int> v(10);
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a(10, v.data());

        ptrOf<int>(a.begin());

        a.setMem(42);

        EXPECT_EQ(a.val(), 42);
        EXPECT_EQ(v[0], 42);
    }

    TEST(ArrayViewTest, SetMemFromVectorTest)
    {
        std::vector<int> v(10);
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a(10, v.data());

        std::vector<int> vo(10, 53);

        a.setMem(vo);

        EXPECT_EQ(a.val(), 53);
        EXPECT_EQ(v[0], 53);

        EXPECT_EQ(a.val(9), 53);
        EXPECT_EQ(v[9], 53);
    }

    TEST(ArrayViewTest, SetMemFromInitListTest)
    {
        std::vector<int> v(3);
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a(3, v.data());

        a.setMem({0, 1, 2});

        EXPECT_EQ(a.val(0), 0);
        EXPECT_EQ(v[0], 0);

        EXPECT_EQ(a.val(1), 1);
        EXPECT_EQ(v[1], 1);

        EXPECT_EQ(a.val(2), 2);
        EXPECT_EQ(v[2], 2);
    }

    TEST(ArrayViewTest, ThrowSizeMissMatch)
    {
        std::vector<int> v(3);
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a(3, v.data());

        EXPECT_THROW(a.setMem({0, 1, 2, 3}), std::length_error);
    }

    TEST(ArrayViewTest, ArrayViewToArrayViewCpy)
    {
        std::vector<int> v1({9,0,2});
        std::vector<int> v2(3);
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a1(3, v1.data());
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a2(3, v2.data());

        a1.copyTo(a2);

        EXPECT_EQ(a2.val(0), 9);
        EXPECT_EQ(a2.val(1), 0);
        EXPECT_EQ(a2.val(2), 2);

        a1.setMem(0);
        a1.setMem(a2);

        EXPECT_EQ(a1.val(0), 9);
        EXPECT_EQ(a1.val(1), 0);
        EXPECT_EQ(a1.val(2), 2);
    }

    TEST(ArrayViewTest, ArrayViewToArrayViewCpyWithSpace)
    {
        std::vector<int> v(6, 0);
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a({3, 2}, v.data());

        a.at(all, 0).setMem(1);

        EXPECT_EQ(a.val(0), 1);
        EXPECT_EQ(a.val(2), 1);
        EXPECT_EQ(a.val(4), 1);
    }
}

