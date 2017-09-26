#include <catch.hpp>
#include <iostream>
#include <ma/ma>

#include <vector>

using namespace std;
using namespace ma;

using namespace ma::array;
using namespace ma::range;

using BArray = BaseArray<int, shape::MShape<Range>>;

TEST_CASE( "BaseArray test", "[BaseArray]" )
{
    // BArray ba(std::initializer_list<SizeT>{5,4}, BArray::AllocT());
    //
    // SECTION( "good size" )
    // {
    //     REQUIRE(ba.size() == (4*5));
    //     REQUIRE(ba.baseSize() == (4*5));
    // }
    //
    // SECTION( "iterator" )
    // {
    //     auto it1 = ba.begin(), it2 = ba.end();
    //
    //     it1 += 4*5;
    //
    //     REQUIRE(it1 == it2);
    // }
    //
    // SECTION( "sparse iterator" )
    // {
    //     auto sub(ba.at(L(3,1), 2));
    //
    //     auto it1 = sub.begin(), it2 = sub.end();
    //
    //     it1 += 2;
    //
    //     REQUIRE(it1 == it2);
    // }


}
