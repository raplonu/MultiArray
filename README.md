<hr>
<h3>About Multi Array</h3>

Current release: V0.1

We recommend the [Multi Array Project Website](http://loha.obspm.fr/g-flash-rtc/) for further information and examples.

Multi Array provide a collection of structures to handle array and more in C++ (and compatible with CUDA):
- [<b><em>Container</em></b>](link)
  - Allocate with default of custom allocator
  - Manage cycle of life of the array
- [<b><em>View</em></b>](link)
  - Manage a reference to an array
  - Handle one of multiple dimensions sub space
  - Provide iterator to browse the array following the shape
- [<b><em>Array</em></b>](link)
  - Merge the concept of the [<em>container</em>](link) and [<em>view</em>](link)

<br><hr>
<h3>A Simple Example</h3>

```C++
#include <ma>
#include <vector>
#include <iostream>
 
int main()
{
    // Allocate 100 int
    ma::Container<int> container(100);

    // Get a 2D view (10x10 int) of container
    ma::MArrayView<int> view({10, 10}, container.ptr());

    // Allocate 20 int initialized at 42
    std::vector<int> vect(30, 42);

    // Select lines from 2 to 14 (not include) with a step of 4 (i.e. 2,6,10) of the view
    auto subView = view.at(ma::L(2,14,4));

    // Copy the vector elements to subView
    subView.setMem(vect);

    // Print 10x10 elements in view
    std::cout << view << std::endl;
}
```

<br><hr>
<h3>Contributors</h3>

Multi Array is developed as an open-source project by [Julien Bernard](http://github.com/raplonu).

<br><hr>
<h3>Open Source License</h3>

Multi Array is available under the "MIT" open-source license:


Copyright (c) 2016-2019 Julien Bernard, https://github.com/raplonu/MultiArray

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
