#include <gtest/gtest.h>
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

namespace
{
    TEST(FunctionTest, TestWithOneArg)
    {
        vector<int> v(5);

        EXPECT_EQ( detail::sizes(v), v.size() );
    }

    TEST(FunctionTest, TestWithMultipleArgs)
    {
        vector<int> v(5);
        list<int> l(5);

        EXPECT_EQ( detail::sizes(v, l, v), v.size() );
    }

    TEST(FunctionTest, TestWithMultipleArgsAndNonSizedArgs)
    {
        vector<int> v(5);

        EXPECT_EQ( detail::sizes(v, 10, Toto()), v.size() );
    }

    TEST(FunctionTest, DetectWhenSizesArentEqual )
    {
        vector<int> v1(5), v2(6);

        EXPECT_THROW(detail::sizes(v1, v2), std::length_error);
    }

    TEST(FunctionTest, FindWithOtherSizesMethod )
    {
        EXPECT_EQ(detail::sizes( Toto() ), 5);
    }
}
