// #include <catch.hpp>
// #include <ma>
//
// #include <vector>
//
// using namespace std;
// using namespace ma;
//
//
// TEST_CASE( "Array", "[Array]" )
// {
//     Pointer<int> p1(3, 1);
//
//     SECTION( "All value is 1" )
//     {
//         REQUIRE(p1.valueAt(0) == 1);
//         REQUIRE(p1.valueAt(1) == 1);
//         REQUIRE(p1.valueAt(2) == 1);
//     }
//
//     Pointer<int> p2(3, vector<int>{4,5,6});
//
//     SECTION( "All value is 1" )
//     {
//         REQUIRE(p2.valueAt(0) == 4);
//         REQUIRE(p2.valueAt(1) == 5);
//         REQUIRE(p2.valueAt(2) == 6);
//     }
//
//     Pointer<int> p3(3, {7,8,9});
//
//     SECTION( "All value is 1" )
//     {
//         REQUIRE(p3.valueAt(0) == 7);
//         REQUIRE(p3.valueAt(1) == 8);
//         REQUIRE(p3.valueAt(2) == 9);
//     }
//
//     Pointer<int> p4(p3);
//
//     SECTION( "All value is 1" )
//     {
//         REQUIRE(p4.valueAt(0) == 7);
//         REQUIRE(p4.valueAt(1) == 8);
//         REQUIRE(p4.valueAt(2) == 9);
//     }
//
// }
