#include <catch.hpp>
#include <ma>

using namespace ma::range;

TEST_CASE( "LinearRange", "[LinearRange]" )
{
    LinearRange lr(4);

    SECTION( "LinearRange base loop" )
    {
        int sum(0);

        for(auto e : lr)
            sum += e;

        REQUIRE( sum == 6 );
        REQUIRE( lr.begin() + 4ul == lr.end());
    }

    SECTION( "LinearRange negative step" )
    {
        LinearRange nlr(10, -1);

        REQUIRE( nlr[-10] == 0 );

    }
}
