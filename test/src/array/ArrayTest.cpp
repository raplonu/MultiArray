#include <catch.hpp>
#include <iostream>
#include <ma/ma>

#include <iostream>
#include <vector>

using namespace std;
using namespace ma;

TEST_CASE( "Array test", "[SArray && MArray]" )
{

    SECTION( "std container mem copy" )
    {
        SArray<int> sa(5, {1,2,3,4,5});
        MArray<int> ma({2,2}, sa.at(1, delay));

        REQUIRE(sa[0] == 1);
        REQUIRE(sa[1] == 2);
        REQUIRE(sa[2] == 3);
        REQUIRE(sa[3] == 4);
        REQUIRE(sa[4] == 5);

        REQUIRE(sa.value(0) == 1);
        REQUIRE(sa.value(1) == 2);
        REQUIRE(sa.value(2) == 3);
        REQUIRE(sa.value(3) == 4);
        REQUIRE(sa.value(4) == 5);

        REQUIRE(sa[0].value() == 1);
        REQUIRE(sa[1].value() == 2);
        REQUIRE(sa[2].value() == 3);
        REQUIRE(sa[3].value() == 4);
        REQUIRE(sa[4].value() == 5);

        REQUIRE(ma.value(0) == 2);
        REQUIRE(ma.value(1) == 3);
        REQUIRE(ma.value(2) == 4);
        REQUIRE(ma.value(3) == 5);

        REQUIRE(ma[0][0].value() == 2);
        REQUIRE(ma[0][1].value() == 3);
        REQUIRE(ma[1][0].value() == 4);
        REQUIRE(ma[1][1].value() == 5);

        REQUIRE(ma[0][0] == 2);
        REQUIRE(ma[0][1] == 3);
        REQUIRE(ma[1][0] == 4);
        REQUIRE(ma[1][1] == 5);

        ma[0][1] = 42;

        REQUIRE(ma[0][1] == 42);
        REQUIRE(sa[2] == 42);

    }
}
