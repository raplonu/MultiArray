#include <gtest/gtest.h>

#include <ma_api/data/DataContainer.h>

using namespace ma;
using namespace ma::data;

namespace
{
    TEST(DataContainerTest, SimplePtr)
    {
        int i, *p(&i);
        DataContainer<int> dc(p, 1);

        EXPECT_EQ(dc.ptr(), p);
    }

    TEST(DataContainerTest, SimpleMove)
    {
        std::unique_ptr<int> p(new int(42));
        int *addr = p.get();

        DataContainer<int> dc(move(p), 1);

        EXPECT_EQ(dc.ptr(), addr);
    }

    TEST(DataContainerTest, VectorTest)
    {
        std::vector<int> v{42};
        int *addr = v.data();

        DataContainer<int> dc(move(v));

        EXPECT_EQ(dc.ptr(), addr);
        EXPECT_EQ(*dc.ptr(), 42);
    }

    TEST(DataContainerTest, PointerOfPointerTest)
    {
        int i = 42;
        std::vector<int*> v{&i};
        int **addr = v.data();

        DataContainer<int*> dc(move(v));

        EXPECT_EQ(dc.ptr(), addr);
        EXPECT_EQ(**dc.ptr(), 42);
    }
}

