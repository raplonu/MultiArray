project(MultiArrayTest)

find_package(GTest REQUIRED)

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

add_executable(MultiArrayTest
    ${MA_TEST_SRC}
)

target_link_libraries(MultiArrayTest
    PRIVATE GTest::GTest GTest::Main MultiArray)


gtest_add_tests(TARGET MultiArrayTest
TEST_SUFFIX .noArgs
TEST_LIST   noArgsTests
)
set_tests_properties(${noArgsTests}   PROPERTIES TIMEOUT 10)