#include <catch.hpp>
#include <ma/ma>

using namespace ma::iterator;

TEST_CASE( "Iterator", "Iterator]" )
{

    Iterator li(5);

    REQUIRE( *li == 5 );

    SECTION( "LinearIterator base addition" )
    {
        ++li;

        REQUIRE( *li == 6 );

        li += -2;

        REQUIRE( *li == 4 );
    }

    SECTION( "Iterator comparaison" )
    {
        Iterator li1(5), li2(5);

        REQUIRE( li1 == li2 );

        li1++;

        REQUIRE( li1 != li2 );

        REQUIRE( li1 == ++li2 );
    }

    SECTION( "Iterator comparaison" )
    {
        Iterator li1(5), li2(5);

        REQUIRE( li1 == li2 );

        li1++;

        REQUIRE( li1 != li2 );

    }

    SECTION( "Iterator step" )
    {
        Iterator li1(5, -1);

        ++li1;

        REQUIRE( *li1 == 4 );

        li1 += -2;

        REQUIRE( *li1 == 6 );

    }
}
