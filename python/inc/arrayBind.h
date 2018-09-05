#ifndef MULTIARRAY_WRAP_ARRAY_BIND_H
#define MULTIARRAY_WRAP_ARRAY_BIND_H

#include <algorithm>
#include <iostream>
#include <wyrm>
#include <ma>
#include <typeList.h>
#include <TypeDescriptor.h>

namespace py = pybind11;

template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}

namespace maw
{
    ma::VectRange strides(const ma::VectRange & range, ma::SizeT sizeT)
    {
        ma::VectRange res(ma::size(range));

        std::transform(range.rbegin(), range.rend(), res.rbegin(),
        [sizeT] (ma::SizeT dim) mutable {return ma::exchange(sizeT, sizeT * dim); } );

        return res;
    }

    template<typename T, template<typename> class Array, typename ArrayT = Array<T>>
    ma::HasRawPtr<T, ArrayT, py::class_<ArrayT>> initBufferClass(py::module& m, const std::string & fullName)
    {
        const auto sizeT = sizeof(T);

        return py::class_<ArrayT>(m ,fullName.data(), py::buffer_protocol())
            .def_buffer([](ArrayT &data)-> py::buffer_info {
                auto shape = ma::shape(data);
                auto stride = strides(shape, sizeT);

                return py::buffer_info(ma::ptrOf(data),
                sizeT,
                py::format_descriptor<T>::format(),
                ma::size(shape),
                {shape},
                {stride}
                );
            });

    }

    template<typename T, template<typename> class Array, typename ArrayT = Array<T>>
    ma::HasNotRawPtr<T, ArrayT, py::class_<ArrayT>> initBufferClass(py::module& m, const std::string & fullName)
    {
        return py::class_<ArrayT>(m, fullName.data());
    }

    
    

    // template<>
    // void bindItemAccess(Class & pyclass)
    // {
    //     pyclass.def("__getitem__", [](ArrayT &a, py::slice slice)
    //     {
    //         size_t start, stop, step, slicelength;
    //         if (!slice.compute(ma::shape(a)[0], &start, &stop, &step, &slicelength))
    //             throw py::error_already_set();
    //         return a.at(ma::L(start, stop, step));
    //     })
    //     .def("__setitem__", [](ArrayT &a, py::slice slice, const T & value)
    //     {
    //         size_t start, stop, step, slicelength;
    //         if (!slice.compute(ma::shape(a)[0], &start, &stop, &step, &slicelength))
    //             throw py::error_already_set();
    //         ma::algorithm(a.at(ma::L(start, stop, step)), value);
    //     }
    //     );
    // }

    template<typename T, template<typename> class Array, typename ArrayT = Array<T>>
    py::class_<ArrayT> bindArray(py::module& m, const std::string & baseName)
    {
        auto fullName = appendName<T>(baseName);

        py::class_<ArrayT> pyclass = initBufferClass<T, Array>(m ,fullName);

        pyclass.def_property_readonly("size", ma::size<ArrayT>);
        pyclass.def_property_readonly("shape", ma::shape<ArrayT>);
        pyclass.def_property_readonly("ndim", [](const ArrayT& a) { return ma::size(ma::shape<ArrayT>(a)); });
        pyclass.def_property_readonly("itemsize", [](const ArrayT&) { return sizeof(T); });
        pyclass.def_property_readonly("nbytes", [](const ArrayT& a) { return sizeof(T) * ma::size(a); });
        pyclass.def("__getitem__", [](const ArrayT& a, std::vector<py::slice> slice)
        {
            std::cout << "slice\n";
            size_t start, stop, step, slicelength;
            if (!slice[0].compute(ma::shape(a)[0], &start, &stop, &step, &slicelength))
                throw py::error_already_set();

            std::cout << start << " " << stop << " " << step <<'\n';

            return 0;

        });

        return pyclass;
    }
} // maw


#endif //MULTIARRAY_WRAP_ARRAY_BIND_H