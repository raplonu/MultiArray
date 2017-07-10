#include <catch.hpp>
#include <ma/ma>

#include <vector>

using namespace std;
using namespace ma;

using namespace ma::shape;
using namespace ma::range;

TEST_CASE( "Linear Space", "[Space]" )
{
    MShape<LinearRange> s({2,3,4});

    SECTION( "Right number of dimension" )
    {
        REQUIRE(s.nbDim() == 3);
        REQUIRE(s.nbActiveDim() == 3);
    }

    SECTION( "Right size" )
    {
        REQUIRE(s.size() == 2 * 3 * 4);
        REQUIRE(s.size() == s.baseSize());
    }

    SECTION( "Position" )
    {
        REQUIRE(s.at(0) == 0);

        REQUIRE(s.at(13) == 13);

        REQUIRE(s.at(s.size() - 1) == (s.size() - 1));
    }

    SECTION( "Right dimension after closing" )
    {
        auto sub = s.subShape(1);

        REQUIRE(sub.nbActiveDim() == 2);

        REQUIRE(sub.baseSize() == 2 * 3 * 4);

        REQUIRE(sub.size() == 3 * 4);
    }

    SECTION( "Sub Position" )
    {
        auto sub = s.subShape(1);

        REQUIRE(sub.at(0) == 12);

        REQUIRE(sub.at(4) == 12 + 4);

        REQUIRE(sub.at(sub.size() - 1) == (12 + sub.size() - 1));
    }

    SECTION( "Right dimension after select" )
    {
        auto sub = s.subShape(1, L(1,3), 2);

        REQUIRE(sub.nbActiveDim() == 1);

        REQUIRE(sub.baseSize() == 2 * 3 * 4);

        REQUIRE(sub.size() == 2);
    }

    SECTION( "Sub Position complex" )
    {
        auto sub = s.subShape(1, L(1,3), 2);

        REQUIRE(sub.at(0) == 18);

        REQUIRE(sub.at(1) == 22);
    }
}

TEST_CASE( "Non linear Space", "[Space]" )
{
    MShape<Range> s({2,3,4});

    SECTION( "Right number of dimension" )
    {
        REQUIRE(s.nbDim() == 3);
        REQUIRE(s.nbActiveDim() == 3);
    }

    SECTION( "Right size" )
    {
        REQUIRE(s.size() == 2 * 3 * 4);
        REQUIRE(s.size() == s.baseSize());
    }

    SECTION( "Right dimension after closing" )
    {
        auto sub = s.subShape(1);

        REQUIRE(sub.nbActiveDim() == 2);

        REQUIRE(sub.baseSize() == 2 * 3 * 4);

        REQUIRE(sub.size() == 3 * 4);
    }

    SECTION( "Right dimension after select" )
    {
        auto sub = s.subShape(1, vector<int>{2,0}, 2);

        REQUIRE(sub.nbActiveDim() == 1);

        REQUIRE(sub.baseSize() == 2 * 3 * 4);

        REQUIRE(sub.size() == 2);
    }

    SECTION( "Sub Position complex" )
    {
        auto sub = s.subShape(1, vector<int>{2,0}, 2);

        REQUIRE(sub.at(0) == 22);

        REQUIRE(sub.at(1) == 14);
    }
}
