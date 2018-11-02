#ifndef MULTIARRAY_WRAP_CONTAINER_INTERFACE_H
#define MULTIARRAY_WRAP_CONTAINER_INTERFACE_H

#include <wyrm>

namespace py = pybind11;

namespace maw
{
    void registerContainer(py::module& m);
} // maw


#endif //MULTIARRAY_WRAP_CONTAINER_INTERFACE_H