#include <catch.hpp>
#include <ma>

#include <vector>

using namespace std;
using namespace ma;

using namespace ma::space;
using namespace ma::range;
using namespace ma::iterator;

TEST_CASE( "SpaceIterator", "[SpaceIterator]" )
{
    Space<LinearRange> s(std::vector<SizeT>{2,3,4});

    SECTION( "Right number of iteraton" )
    {
        SpaceIterator<LinearRange> sit(s);

        SizeT count(0);
        while(!sit.finish())
        {
            ++sit;
            ++count;
        }
        REQUIRE(*sit == 2 * 3 * 4);
        REQUIRE(count == 1);
    }

    SECTION( "Right number of iteraton with stride" )
    {
        std::vector<dimension::Dimension<LinearRange>> dims{{1},LinearRange{1,1},{2}};
        auto sub = s.at(dims);

        SpaceIterator<LinearRange> sit(sub);

        SizeT count(0);
        while(!sit.finish())
        {
            ++sit;
            ++count;
        }
        REQUIRE(*sit == 3 * 4 + 4);
        REQUIRE(count == 1);
    }
}
