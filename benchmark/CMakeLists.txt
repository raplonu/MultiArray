project(MultiArrayBench LANGUAGES CXX)

find_package(benchmark REQUIRED)

add_executable(MultiArrayBench
    src/benchmark.cpp
    src/iterator/IteratorBench.cpp
    src/range/RangeBench.cpp
    )

target_link_libraries(MultiArrayBench PRIVATE MultiArray benchmark pthread)
