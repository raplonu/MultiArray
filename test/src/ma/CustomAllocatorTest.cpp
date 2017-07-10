// #include <catch.hpp>
// #include <ma>
//
// #include <vector>
//
// using namespace std;
// using namespace ma;
//
// template<typename T>
// struct MeAlloc
// {
//     T* allocate(SizeT count)
//     {
//         return new T[count];
//     }
//
//     void deallocate(T * ptr, SizeT count)
//     {
//         delete[] ptr;
//     }
// };
//
// namespace ma
// {
//     namespace container
//     {
//         template<typename T>
//         struct MemCopy<MeAlloc<T>, DefaultAlloc<T>>
//         {
//             template<typename = void> //force template property
//             static void copy(T * dst, T const * src, SizeT length)
//             {
//                 std::memcpy(dst, src, length * sizeof(T));
//             }
//         };
//
//         template<typename T>
//         struct MemCopy<DefaultAlloc<T>, MeAlloc<T>>
//         {
//             template<typename = void> //force template property
//             static void copy(T * dst, T const * src, SizeT length)
//             {
//                 std::memcpy(dst, src, length * sizeof(T));
//             }
//         };
//
//         template<typename T>
//         struct MemCopy<MeAlloc<T>, MeAlloc<T>>
//         {
//             template<typename = void> //force template property
//             static void copy(T * dst, T const * src, SizeT length)
//             {
//                 std::memcpy(dst, src, length * sizeof(T));
//             }
//         };
//
//         template<typename T>
//         struct MemFill<MeAlloc<T>>
//         {
//             template<typename = void> //force template property
//             static void fill(T * dst, T val, SizeT size)
//             {
//                 std::fill_n(dst, size, val);
//             }
//         };
//     }
// }
//
// TEST_CASE( "Custom array", "[Custom array]" )
// {
//     Pointer<int> p1(3, 1);
//
//     BasePointer<int, MeAlloc<int>> bp1(3, p1);
//
//     SECTION( "All value is 1" )
//     {
//         REQUIRE(bp1.valueAt(0) == 1);
//         REQUIRE(bp1.valueAt(1) == 1);
//         REQUIRE(bp1.valueAt(2) == 1);
//     }
//
// }
