#include <catch.hpp>
#include <iostream>
#include <ma/ma>


using namespace std;
using namespace ma;
using namespace ma::container;


TEST_CASE( "Shared Container work like container", "[Shared Container]" )
{
    SharedContainer<int> sc{0,1,2,3,4,5,6};

    SECTION( "can access value" )
    {
        REQUIRE(sc[0] == 0);
        REQUIRE(sc[1] == 1);
        REQUIRE(sc[2] == 2);
        REQUIRE(sc[6] == 6);
    }

    SECTION( "can change value" )
    {
        sc[0] = 666;
        REQUIRE(sc[0] == 666);
    }


    SECTION( "share the data" )
    {
        SharedContainer<int> sc1(sc);
        sc[0] = 888;
        REQUIRE(sc1[0] == 888);
    }

    SECTION( "data is sync in to way" )
    {
        SharedContainer<int> sc1(sc);
        sc1[4] = 128;
        REQUIRE(sc[4] == 128);
    }

}
