/**********************************************************************************
 * Copyright (c) 2016-2019 Julien Bernard, https://github.com/raplonu/MultiArray
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************/
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