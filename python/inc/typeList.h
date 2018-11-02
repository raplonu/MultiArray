#ifndef MULTIARRAY_WRAP_TYPE_H
#define MULTIARRAY_WRAP_TYPE_H

#include <cstdint>
#include <wyrm>

namespace maw
{
    using u8 = uint8_t;
    using i8 = int8_t;

    using u16 = uint16_t;
    using i16 = int16_t;

    using u32 = uint32_t;
    using i32 = int32_t;

    using u64 = uint64_t;
    using i64 = int64_t;

    using f32 = float;
    using f64 = double;

    using MATypeList = TypeList<u8, i8, u16, i16, u32, i32, u64, i64, f32, f64>;
}

#endif  // MULTIARRAY_WRAP_TYPE_H
