#include <gtest/gtest.h>

#include <ma_api/container/Container.h>

using namespace ma;
using namespace ma::container;

namespace
{
    TEST(ContainerTest, SimpleConstructor)
    {
        Container<int> c;

        EXPECT_EQ(c.size(), 0);
        EXPECT_EQ(c.ptr(), nullptr);
    }

    TEST(ContainerTest, ConstructorWithSize)
    {
        SizeT size = 100;

        Container<int> c(size);

        EXPECT_EQ(c.size(), size);
        EXPECT_NE(c.ptr(), nullptr);
    }

    TEST(ContainerTest, CopyConstructor)
    {
        SizeT size = 100;

        Container<int> c1(size);
        Container<int> c2(c1);

        EXPECT_EQ(c2.size(), size);
        EXPECT_NE(c2.ptr(), nullptr);
    }

    TEST(ContainerTest, MoveConstructor)
    {
        SizeT size = 100;

        Container<int> c1(size);

        auto ptr = c1.ptr();

        Container<int> c2(std::move(c1));

        EXPECT_EQ(c2.size(), size);
        EXPECT_EQ(c2.ptr(), ptr);
    }
}

