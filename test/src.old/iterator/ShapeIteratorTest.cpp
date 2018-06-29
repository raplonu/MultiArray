#include <gtest/gtest.h>
#include <vector>

#include <ma/ma>

using namespace std;
using namespace ma;

using namespace ma::shape;
using namespace ma::range;
using namespace ma::iterator;

using Shape = MShape<Range>;

using SIterator = ShapeIterator<typename vector<int>::iterator, Shape>;

namespace
{
    Shape s({2,3,4});
    vector<int> v(2*3*4);
    SIterator it(v.begin(), s, 0);

    TEST(ShapeIteratorTest, GoodPoint)
    {
        EXPECT_EQ(*it, *v.begin());
    }

    TEST(ShapeIteratorTest, SubPositionComplex)
    {
        auto sub = s.subShape(1, vector<int>{1,2,2,0}, 2);
        SIterator sit(v.begin(), sub, 0);

        EXPECT_EQ(sit.pos_at(0), 18);
        ++sit;
        EXPECT_EQ(sit.pos_at(0), 22);
        ++sit;
        EXPECT_EQ(sit.pos_at(0), 22);
        ++sit;
        EXPECT_EQ(sit.pos_at(0), 14);
    }
}
