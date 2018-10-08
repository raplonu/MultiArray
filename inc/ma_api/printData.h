#ifndef MA_PRINT_DATA_H
#define MA_PRINT_DATA_H

#include <ostream>
#include <ma_api/range/rangeFunction.h>

namespace ma
{
    template<typename Data>
    void printLine(std::ostream& s, const Data & data)
    {
        auto fi = ma::begin(data), la = ma::end(data);
        if(fi != la)
        {
            s << (*fi);
            while(++fi != la)
            {
                s << ", " << *fi;
            }
        }
    }

    template <typename Data>
    void printData(std::ostream& s, const Data & data)
    {
        auto shape = ma::shape(data);
        if(ma::size(shape) == 1)
        {
            printLine(s, data);
        }
        else
            for(auto index : ma::range::L(shape[0]))
            {
                s.put('[');
                printData(s, data[index]);
                s << "]\n";
            }

    }
}

#endif //MA_PRINT_DATA_H