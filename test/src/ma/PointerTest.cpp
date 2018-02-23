// #include <gtest/gtest.h>
// #include <ma>
//
// #include <vector>
//
// using namespace std;
// using namespace ma;
//
//
// namespace
// {
//     Pointer<int> p1(3, 1);
//
//     TEST( "All value is 1" )
//     {
//         EXPECT_EQ(p1.valueAt(0), 1);
//         EXPECT_EQ(p1.valueAt(1), 1);
//         EXPECT_EQ(p1.valueAt(2), 1);
//     }
//
//     Pointer<int> p2(3, vector<int>{4,5,6});
//
//     TEST( "All value is 1" )
//     {
//         EXPECT_EQ(p2.valueAt(0), 4);
//         EXPECT_EQ(p2.valueAt(1), 5);
//         EXPECT_EQ(p2.valueAt(2), 6);
//     }
//
//     Pointer<int> p3(3, {7,8,9});
//
//     TEST( "All value is 1" )
//     {
//         EXPECT_EQ(p3.valueAt(0), 7);
//         EXPECT_EQ(p3.valueAt(1), 8);
//         EXPECT_EQ(p3.valueAt(2), 9);
//     }
//
//     Pointer<int> p4(p3);
//
//     TEST( "All value is 1" )
//     {
//         EXPECT_EQ(p4.valueAt(0), 7);
//         EXPECT_EQ(p4.valueAt(1), 8);
//         EXPECT_EQ(p4.valueAt(2), 9);
//     }
//
// }
