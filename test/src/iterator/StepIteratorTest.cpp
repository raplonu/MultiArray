#include <catch.hpp>
#include <iostream>
#include <ma/ma>

#include <vector>

using namespace std;
using namespace ma;

TEST_CASE( "step iterator test", "[StepIterator]" )
{

    // SECTION( "simple it with std comtainer" )
    // {
    //     vector<int> v1{1,2,3,4};
    //
    //     auto it(ma::iterator::stepIterator<int>(v1, 1));
    //
    //     REQUIRE(*it == 1);
    //     ++it;
    //     REQUIRE(*it == 2);
    //     ++it;
    //     REQUIRE(*it == 3);
    //     ++it;
    //     REQUIRE(*it == 4);
    // }
    //
    // SECTION( "step it with std comtainer" )
    // {
    //     vector<int> v1{1,2,3,4,5,6};
    //
    //     auto it(ma::iterator::stepIterator<int>(v1, 2));
    //
    //     REQUIRE(*it == 1);
    //     ++it;
    //     REQUIRE(*it == 3);
    //     ++it;
    //     REQUIRE(*it == 5);
    // }

    SECTION( "const it with value" )
    {
        auto it(ma::iterator::stepIterator<const int>(int(8), 1));

        REQUIRE(detail::convert<const int>(it) == 8);
        ++it;
        REQUIRE(detail::convert<const int>(it) == 8);
        ++it;
        REQUIRE(detail::convert<const int>(it) == 8);
        ++it;
        REQUIRE(detail::convert<const int>(it) == 8);
    }

}
