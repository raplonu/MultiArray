#include <catch.hpp>
#include <ma>

using namespace ma::iterator;

TEST_CASE( "LinearIterator", "[LinearIterator]" )
{

    LinearIterator li(5);

    REQUIRE( li.value() == 5 );

    SECTION( "LinearIterator base addition" )
    {
        ++li;

        REQUIRE( li.value() == 6 );

        li += -2;

        REQUIRE( li.value() == 4 );
    }

    SECTION( "LinearIterator comparaison" )
    {
        LinearIterator li1(5), li2(5);

        REQUIRE( li1 == li2 );

        li1++;

        REQUIRE( li1 > li2 );

        REQUIRE( li1 == ++li2 );
    }

    SECTION( "LinearIterator comparaison" )
    {
        LinearIterator li1(5), li2(5);

        REQUIRE( li1 == li2 );

        li1++;

        REQUIRE( li1 > li2 );

    }
}
