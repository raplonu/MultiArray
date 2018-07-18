#include <gtest/gtest.h>

#include <ma/array/ArrayView.h>
#include <ma/dimension/BasicShape.h>
#include <ma/dimension/MultiShape.h>
#include <ma/range/LinearRange.h>

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

    TEST(ArrayViewTest, MultiArrayView1Dim)
    {
        std::vector<int> v(10);
        ArrayView<int, DefaultAlloc<int>, MultiShape<LinearRange>> a(10, v.data());

        EXPECT_EQ(a.ptr(), v.data());
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

        a.setMem(42);
        
        EXPECT_EQ(a.value(0), 42);
        EXPECT_EQ(v[0], 42);
    }
}

