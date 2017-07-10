#include <catch.hpp>
#include <ma/ma>
#include <vector>

using namespace ma::range;
using namespace std;

TEST_CASE( "Range Test", "Range" )
{

    Range r1(vector<ma::SizeT>{0,0,1,1,2,2});

    SECTION( "Range size" )
    {
        REQUIRE( r1.size() == 6 );
    }

    SECTION( "Range properties" )
    {
        REQUIRE( !r1.isComplete(6) );

        REQUIRE( r1.isActive() );
    }

    Range r2(r1.select(Range(0,6,2)));

    SECTION( "Range size" )
    {
        REQUIRE( r2.size() == 3 );
    }

    SECTION( "Range properties" )
    {
        REQUIRE( r2.isComplete(3) );

        REQUIRE( r2.isActive() );
    }



    // Dimension<LinearRange> r3(r2.closeAt(3));
    //
    // SECTION( "Dimension size" )
    // {
    //     REQUIRE( d3.size() == 0 );
    //
    //     REQUIRE( d3.realSize() == 5 );
    // }
    //
    // SECTION( "Dimension properties" )
    // {
    //     REQUIRE( !d3.isComplete() );
    //
    //     REQUIRE( !d3.isActive() );
    // }
    //
    // SECTION( "Dimension pos" )
    // {
    //     REQUIRE( *(d3.begin()) == 3 );
    //
    // }
}
