#include <gtest/gtest.h>

#include <ma_api/container/SharedContainer.h>

using namespace ma;
using namespace ma::container;

namespace
{
    TEST(SharedContainerTest, SimpleConstructor)
    {
        SharedContainer<int> c;

        EXPECT_EQ(c.size(), 0);
        EXPECT_EQ(c.data(), nullptr);
    }

    TEST(SharedContainerTest, ConstructorWithSize)
    {
        SizeT size = 100;

        SharedContainer<int> c(size);

        EXPECT_EQ(c.size(), size);
        EXPECT_NE(c.data(), nullptr);
    }

    TEST(SharedContainerTest, CopyConstructor)
    {
        SizeT size = 100;

        SharedContainer<int> c1(size);
        SharedContainer<int> c2(c1);

        EXPECT_EQ(c2.size(), size);
        EXPECT_NE(c2.data(), nullptr);
    }

    TEST(SharedContainerTest, MoveConstructor)
    {
        SizeT size = 100;

        SharedContainer<int> c1(size);

        auto ptr = c1.data();

        SharedContainer<int> c2(std::move(c1));

        EXPECT_EQ(c2.size(), size);
        EXPECT_EQ(c2.data(), ptr);
    }
}

