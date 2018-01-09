// #include <gtest/gtest.h>
// #include <iostream>
// #include <ma/ma>

// #include <vector>

// using namespace std;
// using namespace ma;

// using namespace ma::array;
// using namespace ma::range;

// using BArray = BaseArray<int, shape::MShape<Range>>;

// namespace
// {
//     BArray ba(std::initializer_list<SizeT>{5,4}, BArray::AllocT());
    
//     TEST( "good size" )
//     {
//         EXPECT_EQ(ba.size(), (4*5));
//         EXPECT_EQ(ba.baseSize(), (4*5));
//     }
    
//     TEST( "iterator" )
//     {
//         auto it1 = ba.begin(), it2 = ba.end();
    
//         it1 += 4*5;
    
//         EXPECT_EQ(it1, it2);
//     }
    
//     TEST( "sparse iterator" )
//     {
//         auto sub(ba.at(L(3,1), 2));
    
//         auto it1 = sub.begin(), it2 = sub.end();
    
//         it1 += 2;
    
//         EXPECT_EQ(it1, it2);
//     }


// }
