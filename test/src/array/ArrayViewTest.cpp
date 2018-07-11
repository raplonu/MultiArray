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
}

