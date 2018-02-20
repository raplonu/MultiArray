cmake_minimum_required(VERSION 3.6)

# Global option #
#################

if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-W -Wall)
endif()

set (CMAKE_CXX_STANDARD 11)

# MultiArray #
##############

project(MultiArray VERSION 0.1 LANGUAGES CXX)

add_library(MultiArray INTERFACE)

target_include_directories(MultiArray INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>  
    $<INSTALL_INTERFACE:inc>
)

enable_testing()
add_subdirectory(test)


# Install #
###########

install(TARGETS MultiArray EXPORT MultiArrayTargets)

install(DIRECTORY inc/ma DESTINATION include)

install(EXPORT MultiArrayTargets DESTINATION lib/cmake/MultiArray)

export(TARGETS MultiArray FILE MultiArrayTargets.cmake)

