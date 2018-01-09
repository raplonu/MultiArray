
project(MA_TEST)

find_package(GTest)

enable_testing()

set(MA_TEST_SRC
    src/array/ArrayTest.cpp
    src/array/BaseArrayTest.cpp
    src/container/SharedContainerTest.cpp
    src/copy/memCopyTest.cpp
    src/detail/functionTest.cpp
    src/dimension/DimensionTest.cpp
    src/iterator/IteratorTest.cpp
    src/iterator/LinearIteratorTest.cpp
    src/iterator/ShapeIteratorTest.cpp
    src/iterator/StepIteratorTest.cpp
    src/ma/ArrayTest.cpp
    src/ma/CustomAllocatorTest.cpp
    src/ma/PointerTest.cpp
    src/range/LinearRangeTest.cpp
    src/range/RangeTest.cpp
    src/shape/MShapeTest.cpp
    src/shape/SimpleShapeTest.cpp
)

add_executable(MA_TEST
    ${MA_TEST_SRC}
)


target_link_libraries(MA_TEST
    PRIVATE GTest::GTest GTest::Main MultiArray)