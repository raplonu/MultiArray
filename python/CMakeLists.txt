cmake_minimum_required(VERSION 3.6)

if(NOT TARGET wyrm)
    if(NOT DEFINED WYRM_ROOT)
        set(WYRM_ROOT $ENV{WYRM_ROOT})
    endif()
    if(IS_DIRECTORY ${WYRM_ROOT})
        add_subdirectory(${WYRM_ROOT} wyrm_dir)
    else()
        message( FATAL_ERROR "wyrm path not found, specify it by set WYRM_ROOT to the right path" )
    endif()
endif()

project(MultiArrayWrap LANGUAGES CXX)

add_library(MultiArrayWrap MODULE
    src/MultiArrayWrap.cpp
    src/ContainerInterface.cpp
)

target_include_directories(MultiArrayWrap PRIVATE inc)

target_link_libraries(MultiArrayWrap PRIVATE wyrm MultiArray)

set_target_properties(MultiArrayWrap PROPERTIES
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}")
