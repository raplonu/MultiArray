#include <wyrm>

#include <ma>
#include <ContainerInterface.h>
#include <typeList.h>
#include <TypeDescriptor.h>

namespace maw
{
    struct ContainerInterfacer
    {
        template<typename T>
        static void call(py::module& m)
        {
            auto name = appendName<T>("Container");
            const auto sizeT = sizeof(T);
            using Class = ma::container::Container<T>;

            py::class_<Class>(m ,name.data(), py::buffer_protocol())
                .def(py::init())
                .def(py::init<int>())
                .def_buffer([](Class &data)-> py::buffer_info {
                    return py::buffer_info(data.data(),
                    sizeT,
                    py::format_descriptor<T>::format(),
                    1,
                    {data.size()},
                    {sizeT}
                    );
                })
                .def_property_readonly("size", &Class::size);
        }
    };


    void registerContainer(py::module& m)
    {
    apply<ContainerInterfacer, MATypeList>(m);    
    } 
} // maw