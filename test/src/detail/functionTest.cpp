#include <catch.hpp>
#include <ma/ma>

#include <vector>
#include <list>

using namespace std;
using namespace ma;

struct Toto
{
    unsigned int syze() const { return 5; }
};

unsigned int size(Toto const & t){ return t.syze(); }

TEST_CASE( "Sizes find size's args", "[sizes]" )
{
    SECTION( "Test with one arg" )
    {
        vector<int> v(5);

        REQUIRE( detail::sizes(v) == v.size() );
    }

    SECTION( "Test with multiple args" )
    {
        vector<int> v(5);
        list<int> l(5);

        REQUIRE( detail::sizes(v, l, v) == v.size() );
    }

    SECTION( "Test with multiple args and non sized args" )
    {
        vector<int> v(5);

        REQUIRE( detail::sizes(v, 10, Toto()) == v.size() );
    }

    SECTION( "Detect when sizes aren't equal" )
    {
        vector<int> v1(5), v2(6);

        REQUIRE_THROWS_AS(detail::sizes(v1, v2), std::length_error);
    }

    SECTION( "Find with other sizes method" )
    {
        REQUIRE(detail::sizes( Toto() ) == 5);
    }
}
