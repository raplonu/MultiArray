cmake_minimum_required(VERSION 3.6)

if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-W -Wall)
endif()

set (CMAKE_CXX_STANDARD 11)

add_library(MultiArray INTERFACE)

#target_sources(MultiArray INTERFACE "${PROJECT_SOURCE_DIR}/inc/ma/ma")

target_include_directories(MultiArray INTERFACE
    "${PROJECT_SOURCE_DIR}/inc"
)

add_subdirectory(test)