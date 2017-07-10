#include <catch.hpp>
#include <iostream>
#include <ma/ma>

using namespace std;
using namespace ma;
using namespace ma::shape;


TEST_CASE( "Simple Shape test", "[Simple Shape]" )
{
    SimpleShape ss{100};

    SECTION( "good size" )
    {
        REQUIRE(ss.size() == 100);
        REQUIRE(ss.baseSize() == 100);

        REQUIRE(ss.at(0) == 0);
        REQUIRE(ss.at(99) == 99);
    }

    SECTION( "make sub shape" )
    {
        SimpleShape ss1(ss.subShape(10, 90));

        REQUIRE(ss1.size() == 80);
        REQUIRE(ss1.baseSize() == 100);

        REQUIRE(ss1.at(0) == 10);
        REQUIRE(ss1.at(79) == 89);
    }

    SECTION( "can reset" )
    {
        SimpleShape ss1(ss.subShape(10, 90));
        ss1.reset();

        REQUIRE(ss1.size() == 100);
        REQUIRE(ss1.baseSize() == 100);

        REQUIRE(ss1.at(0) == 0);
        REQUIRE(ss1.at(99) == 99);
    }
}
