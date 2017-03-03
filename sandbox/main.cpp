#include <iostream>
#include "../inc/range/Range.h"
#include "../inc/dimension/Dimension.h"
#include "../inc/space/Space.h"

#include <vector>

using namespace std;
using namespace ma;
using namespace ma::range;
using namespace ma::dimension;
using namespace ma::space;

template<typename T>
static std::ostream& operator<<(std::ostream& s, const std::vector<T>& v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v) {
    s << comma << e;
    comma[0] = ',';
    }
    return s << ']';
}

template<typename T>
void print(T range)
{
    cout << "Size : " << range.size() << endl;
    for(auto e : range)
        cout << e << " ";
    cout << endl;
}

int main()
{
    // auto lr1(LinearRange(10,0,-2));
    // auto lr2(LinearRange(2,4));
    // print(lr1);
    //
    // print(lr1.select(lr2));

    auto lri_open(RangeInterface(4,5));
    auto lri_close(RangeInterface(4,4));
    // print(lri);

    auto vri1(RangeInterface(vector<SizeT>{4,5,6}));
    // auto vri2(RangeInterface(vector<SizeT>{4,3,2,1}));
    // print(vri1);
    //
    // print(lri.select(vri1));
    // print(vri1.select(vri2));

    cout << "Dimension :\n";

    Dimension<RangeInterface> dri(20);
    print(dri);
    print(dri.select(Dimension<RangeInterface>(lri_open)));
    print(dri.select(Dimension<RangeInterface>(lri_close)));
    dri.reset();
    print(dri);

    Space<RangeInterface> s(vector<SizeT>{2,3,4});

    cout << s.size() << " " << s.realSize() << endl;

    return 0;
}
