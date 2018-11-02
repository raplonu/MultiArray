#ifndef MULTIARRAY_WRAP_TYPE_DESCRIPTOR_H
#define MULTIARRAY_WRAP_TYPE_DESCRIPTOR_H

#include <typeList.h>

namespace maw
{


  template <typename T>
  static char const* name();

  #define DECLARE_NAME(Type, Name)       \
    template <>                          \
    constexpr char const* name<Type>() { \
      return #Name;                      \
    }

  DECLARE_NAME(u8, Uint8)
  DECLARE_NAME(i8, Int8)

  DECLARE_NAME(u16, Uint16)
  DECLARE_NAME(i16, Int16)

  DECLARE_NAME(u32, Uint32)
  DECLARE_NAME(i32, Int32)

  DECLARE_NAME(u64, Uint64)
  DECLARE_NAME(i64, Int64)

  // DECLARE_NAME(f16, Half);
  DECLARE_NAME(f32, Float);
  DECLARE_NAME(f64, Double);

  template <typename T>
  std::string appendName(std::string str) {
    return str + name<T>();
  }

 
}

#endif  // MULTIARRAY_WRAP_TYPE_DESCRIPTOR_H
