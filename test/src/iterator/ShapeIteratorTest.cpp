#include <catch.hpp>
#include <ma/ma>

#include <vector>

using namespace std;
using namespace ma;

using namespace ma::shape;
using namespace ma::range;
using namespace ma::iterator;

using Shape = MShape<Range>;

using SIterator = ShapeIterator<typename vector<int>::iterator, Shape>;

TEST_CASE( "Shape iterator test", "[Shape iterator]" )
{
    Shape s({2,3,4});
    vector<int> v(2*3*4);
    SIterator it(v.begin(), s, 0);

    SECTION( "good point" )
    {
        REQUIRE(*it == *v.begin());
    }

    SECTION( "Sub Position complex" )
    {
        auto sub = s.subShape(1, vector<int>{1,2,2,0}, 2);
        SIterator sit(v.begin(), sub, 0);

        REQUIRE(sit.pos_at(0) == 18);
        ++sit;
        REQUIRE(sit.pos_at(0) == 22);
        ++sit;
        REQUIRE(sit.pos_at(0) == 22);
        ++sit;
        REQUIRE(sit.pos_at(0) == 14);
    }
}
