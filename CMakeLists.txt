cmake_minimum_required(VERSION 3.6)

# Global option #
#################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-W -Wall)
endif()

# MultiArray #
##############

project(MultiArray VERSION 0.1 LANGUAGES CXX)

add_library(MultiArray INTERFACE)

target_compile_features(MultiArray INTERFACE cxx_std_11)

target_include_directories(MultiArray INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>  
    $<INSTALL_INTERFACE:inc>
)

########################################################################
#
# Google Test.
#
# The tests are not built by default.  To build them, set the
# gtest_build_tests option to ON.  You can do it by running ccmake
# or specifying the -Dgtest_build_tests=ON flag when running cmake.

if (gtest_build_tests)
  # This must be set in the root directory for the tests to be run by
  # 'make test' or ctest.
  enable_testing()
  add_subdirectory(test)
endif()

if(benchmark_build)
    add_subdirectory(benchmark)
endif()

# Install #
###########

install(TARGETS MultiArray EXPORT MultiArrayTargets)

install(DIRECTORY inc/ma DESTINATION include)

install(EXPORT MultiArrayTargets DESTINATION lib/cmake/MultiArray)

export(TARGETS MultiArray FILE MultiArrayTargets.cmake)

