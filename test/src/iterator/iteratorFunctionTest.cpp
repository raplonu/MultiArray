#include <gtest/gtest.h>

#include <vector>

#include <ma_api/iterator/LinearIterator.h>
#include <ma_api/iterator/iteratorFunction.h>

using namespace ma;
using namespace ma::iterator;

namespace
{
    TEST(IteratorId, CompareLinear)
    {
        LinearIterator l1(1), l2(2);

        EXPECT_NE(*l1, *l2);
        EXPECT_NE(iteratorId(l1), iteratorId(l2));
        l1++;
        EXPECT_EQ(*l1, *l2);
        EXPECT_EQ(iteratorId(l1), iteratorId(l2));
    }

    TEST(IteratorId, CompareCustom)
    {
        std::vector<int> v{1,1};
        auto it1 = v.begin();
        auto it2 = it1 + 1;

        EXPECT_EQ(*it1, *it2);
        EXPECT_NE(iteratorId(it1), iteratorId(it2));
        it1++;
        EXPECT_EQ(*it1, *it2);
        EXPECT_EQ(iteratorId(it1), iteratorId(it2));
    }
}

