#include <catch.hpp>
#include <iostream>
#include <ma/ma>

#include <vector>

using namespace std;
using namespace ma;

TEST_CASE( "memCopyPlain test", "[memCopy]" )
{

    SECTION( "std container mem copy" )
    {
        vector<int> v1 (3,1), v2 (3,0);
        copy::memCopy<int>(v2, v1);

        REQUIRE(v2[0] == 1);
        REQUIRE(v2[1] == 1);
        REQUIRE(v2[2] == 1);
    }

    SECTION( "std container fill" )
    {
        vector<int> v (3,0);
        copy::memCopy<int>(v, 42);

        REQUIRE(v[0] == 42);
        REQUIRE(v[1] == 42);
        REQUIRE(v[2] == 42);
    }

    SECTION( "std pointer mem copy" )
    {
        vector<int> v1 (3,1), v2 (4,0);
        int *p(v2.data());
        copy::memCopy<int>(p, v1);

        REQUIRE(v2[0] == 1);
        REQUIRE(v2[1] == 1);
        REQUIRE(v2[2] == 1);
        REQUIRE(v2[3] == 0);
    }

    SECTION( "std array mem copy" )
    {
        vector<int> v1 (3,1);
        SArray<int> sa(4);

        SArray<int> sub = sa.at(0, 3);

        copy::memCopy<int>(sub, v1);

        REQUIRE(sa.value(0) == 1);
        REQUIRE(sa.value(1) == 1);
        REQUIRE(sa.value(2) == 1);
        REQUIRE(sa.value(3) == 0);
    }
}

TEST_CASE( "memCopyStep test", "[memCopy]" )
{

    SECTION( "std container mem copy" )
    {
        vector<int> v1 (3,1), v2 (3,0);
        copy::memCopyStep<int>(v2, v1);

        REQUIRE(v2[0] == 1);
        REQUIRE(v2[1] == 1);
        REQUIRE(v2[2] == 1);
    }

    SECTION( "std container fill" )
    {
        vector<int> v (3,0);
        copy::memCopyStep<int>(v, 42);

        REQUIRE(v[0] == 42);
        REQUIRE(v[1] == 42);
        REQUIRE(v[2] == 42);
    }

    SECTION( "std pointer mem copy" )
    {
        vector<int> v1 (3,1), v2 (4,0);
        int *p(v2.data());
        copy::memCopyStep<int>(p, v1);

        REQUIRE(v2[0] == 1);
        REQUIRE(v2[1] == 1);
        REQUIRE(v2[2] == 1);
        REQUIRE(v2[3] == 0);
    }

    SECTION( "std array mem copy" )
    {
        vector<int> v1 (3,1);
        MArray<int> sa(6);

        MArray<int> sub = sa.at(L(0, 6, 2));

        copy::memCopyStep<int>(sub, v1);

        REQUIRE(sa.value(0) == 1);
        REQUIRE(sa.value(1) == 0);
        REQUIRE(sa.value(2) == 1);
        REQUIRE(sa.value(3) == 0);
        REQUIRE(sa.value(4) == 1);
        REQUIRE(sa.value(5) == 0);
    }

    SECTION( "multi dim std array mem copy" )
    {
        vector<int> v1 (4,1);
        MArray<int> sa({4,4}, 0);

        MArray<int> sub = sa.at(L(1, 3), L(1, 3));

        copy::memCopyStep<int>(sub, v1);

        REQUIRE(sa.value(0) == 0);
        REQUIRE(sa.value(1) == 0);
        REQUIRE(sa.value(2) == 0);
        REQUIRE(sa.value(3) == 0);
        REQUIRE(sa.value(4) == 0);
        REQUIRE(sa.value(5) == 1);
        REQUIRE(sa.value(6) == 1);
        REQUIRE(sa.value(7) == 0);
        REQUIRE(sa.value(8) == 0);
        REQUIRE(sa.value(9) == 1);
        REQUIRE(sa.value(10) == 1);
        REQUIRE(sa.value(11) == 0);
        REQUIRE(sa.value(12) == 0);
        REQUIRE(sa.value(13) == 0);
        REQUIRE(sa.value(14) == 0);
        REQUIRE(sa.value(15) == 0);
    }
}
