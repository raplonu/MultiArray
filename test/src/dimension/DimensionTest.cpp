#include <catch.hpp>
#include <ma/ma>

#include <vector>
#include <iostream>

using namespace ma::dimension;
using namespace ma::range;
using namespace std;

TEST_CASE( "Dimension<LinearRange> Test", "[Dimension<LinearRange>]" )
{

    Dimension<LinearRange> d1(5);

    SECTION( "Dimension size" )
    {
        REQUIRE( d1.size() == 5 );

        REQUIRE( d1.baseSize() == 5 );
    }

    SECTION( "Dimension properties" )
    {
        REQUIRE( d1.isComplete() );

        REQUIRE( d1.isActive() );
    }

    Dimension<LinearRange> d2(d1.select(Dimension<LinearRange>(3)));

    SECTION( "Dimension size" )
    {
        REQUIRE( d2.size() == 3 );

        REQUIRE( d2.baseSize() == 5 );
    }

    SECTION( "Dimension properties" )
    {
        REQUIRE( !d2.isComplete() );

        REQUIRE( d2.isActive() );
    }

    Dimension<LinearRange> d3(d2.closeAt(3));

    SECTION( "Dimension size" )
    {
        REQUIRE( d3.size() == 0 );

        REQUIRE( d3.baseSize() == 5 );
    }

    SECTION( "Dimension properties" )
    {
        REQUIRE( !d3.isComplete() );

        REQUIRE( !d3.isActive() );
    }

    SECTION( "Dimension pos" )
    {
        REQUIRE( *(d3.begin()) == 3 );

    }
}

TEST_CASE( "Dimension<Range> Test", "[Dimension<Range>]" )
{

    Dimension<Range> d1(Range(2,7));

    SECTION( "Dimension size" )
    {
        REQUIRE( d1.size() == 5 );

        REQUIRE( d1.baseSize() == 5 );
    }

    SECTION( "Dimension properties" ) 
    {
        REQUIRE( !d1.isComplete() );

        REQUIRE( d1.isActive() );
    }

    Dimension<Range> dTmp(Range(std::vector<ma::SizeT>{3,2,1}));

    Dimension<Range> d2(d1.select(dTmp));

    SECTION( "Dimension size" )
    {
        REQUIRE( d2.size() == 3 );

        REQUIRE( d2.baseSize() == 5 );
    }

    SECTION( "Dimension properties" )
    {
        REQUIRE( !d2.isComplete() );

        REQUIRE( d2.isActive() );
    }

    Dimension<Range> d3(d2.closeAt(1));

    SECTION( "Dimension size" )
    {
        REQUIRE( d3.size() == 0 );

        REQUIRE( d3.baseSize() == 5 );
    }

    SECTION( "Dimension properties" )
    {
        REQUIRE( !d3.isComplete() );

        REQUIRE( !d3.isActive() );
    }

    SECTION( "Dimension pos" )
    {
        REQUIRE( *(d3.begin()) == 4 );

    }

    SECTION( "Dimension it" )
    {
        REQUIRE( (++d3.begin()) == d3.end() );
    }

    Dimension<Range> d4(Range(std::vector<ma::SizeT>{1}));

    SECTION( "Dimension it" )
    {
        REQUIRE( (++d4.begin()) == d4.end() );
    }
}
