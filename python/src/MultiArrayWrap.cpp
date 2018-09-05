#include <wyrm>
#include <ma>

// #include <ContainerInterface.h>
#include <arrayBind.h>

namespace py = pybind11;

template<typename T>
using MeContainer = ma::container::Container<T>;

template<typename T>
using MeMArray = ma::MArray<T>;

PYBIND11_MODULE(MultiArrayWrap, m) {
  m.doc() = "MultiArray interface module";

  // maw::registerContainer(m);
  // maw::bindArray<int, MeContainer>(m, "Container")
  // .def(py::init<int>());

  maw::bindArray<int, MeMArray>(m, "MArray")
  .def(py::init<int>())
  .def(py::init<ma::VectRange>());
}