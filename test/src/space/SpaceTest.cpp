#include <catch.hpp>
#include <ma>

#include <vector>

using namespace std;
using namespace ma;

using namespace ma::space;
using namespace ma::range;

TEST_CASE( "Space", "[Space]" )
{
    Space<LinearRange> s(std::vector<SizeT>{2,3,4});

    SECTION( "Right number of dimension" )
    {
        REQUIRE(s.nbDim() == 3);
        REQUIRE(s.nbActiveDim() == 3);
    }

    SECTION( "Right size" )
    {
        REQUIRE(s.size() == 2 * 3 * 4);
        REQUIRE(s.size() == s.realSize());
    }

    SECTION( "Right dimension after closing" )
    {
        auto sub = s[1];

        REQUIRE(sub.nbActiveDim() == 2);

        REQUIRE(sub.realSize() == 2 * 3 * 4);

        REQUIRE(sub.size() == 3 * 4);
    }

    SECTION( "Right dimension after select" )
    {
        std::vector<dimension::Dimension<LinearRange>> dims{{1},LinearRange{1,1},{2}};
        auto sub = s.at(dims);

        REQUIRE(sub.nbActiveDim() == 2);

        REQUIRE(sub.realSize() == 2 * 3 * 4);

        REQUIRE(sub.size() == 2);
    }
}
