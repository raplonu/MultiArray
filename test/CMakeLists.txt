project(MultiArrayTest LANGUAGES CXX)

find_package(GTest REQUIRED)

set(MA_TEST_SRC
    src/container/ContainerTest.cpp
    src/container/SharedContainerTest.cpp
    src/iterator/LinearIteratorTest.cpp
    src/iterator/iteratorFunctionTest.cpp
    src/iterator/IteratorTest.cpp
    src/range/LinearRangeTest.cpp
    src/range/rangeFunctionTest.cpp
    src/range/RangeTest.cpp
    src/dimension/DimensionTest.cpp
    src/dimension/BasicShapeTest.cpp
    src/dimension/dimensionFunctionTest.cpp
    src/array/ArrayViewTest.cpp
)

add_executable(MultiArrayTest
    ${MA_TEST_SRC}
)

target_compile_features(MultiArrayTest PRIVATE cxx_std_11)

target_link_libraries(MultiArrayTest
    PRIVATE GTest::GTest GTest::Main MultiArray)


gtest_add_tests(TARGET MultiArrayTest
TEST_SUFFIX .noArgs
TEST_LIST   noArgsTests
)
set_tests_properties(${noArgsTests}   PROPERTIES TIMEOUT 10)