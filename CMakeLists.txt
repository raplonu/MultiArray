cmake_minimum_required(VERSION 3.6)

# Global option #
#################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-W -Wall)
endif()

# multiarray #
##############

project(MultiArray VERSION 0.1 LANGUAGES CXX)

add_library(MultiArray INTERFACE)

target_compile_features(MultiArray INTERFACE cxx_std_14)

target_include_directories(MultiArray INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>  
    $<INSTALL_INTERFACE:include>
)


########################################################################
#
# Python wrap. 
#
# The python wrapper is not built by default.  To build it, set the
# python_build option to ON.  You can do it by running ccmake
# or specifying the -Dpython_build=ON flag when running cmake.

if(python_build)
    add_subdirectory(python)
endif()


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

########################################################################
#
# Google Benchmark.
#
# The bench are not built by default.  To build them, set the
# benchmark_build option to ON.  You can do it by running ccmake
# or specifying the -Dbenchmark_build=ON flag when running cmake.

if(benchmark_build)
    add_subdirectory(benchmark)
endif()

# Install #
###########

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/MultiArray)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MultiArrayConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(TARGETS MultiArray
    EXPORT MultiArrayTargets
    LIBRARY DESTINATION lib COMPONENT Runtime
    ARCHIVE DESTINATION lib COMPONENT Development
    RUNTIME DESTINATION bin COMPONENT Runtime
    PUBLIC_HEADER DESTINATION include COMPONENT Development
    BUNDLE DESTINATION bin COMPONENT Runtime
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/MultiArrayConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/MultiArrayConfig.cmake"
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(EXPORT MultiArrayTargets DESTINATION ${INSTALL_CONFIGDIR})
install(FILES "${PROJECT_BINARY_DIR}/MultiArrayConfig.cmake"
              "${PROJECT_BINARY_DIR}/MultiArrayConfig.cmake"
        DESTINATION ${INSTALL_CONFIGDIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/inc/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})




# include(GNUInstallDirs)
# set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/MultiArray)

# install(TARGETS multiarray
#     EXPORT multiarray-targets
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

# #This is required so that the exported target has the name JSONUtils and not jsonutils
# set_target_properties(multiarray PROPERTIES EXPORT_NAME MultiArray)

# install(DIRECTORY inc/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install(EXPORT multiarray-targets
#   FILE
#   MultiArrayTargets.cmake
#   NAMESPACE
#   MultiArray::
#   DESTINATION
#   ${INSTALL_CONFIGDIR}
# )

# #Create a ConfigVersion.cmake file
# include(CMakePackageConfigHelpers)
# write_basic_package_version_file(
#     ${CMAKE_CURRENT_BINARY_DIR}/MultiArrayConfigVersion.cmake
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY AnyNewerVersion
# )

# configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/MultiArrayConfig.cmake.in
#     ${CMAKE_CURRENT_BINARY_DIR}/MultiArrayConfig.cmake
#     INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
# )

# #Install the config, configversion and custom find modules
# install(FILES
#     ${CMAKE_CURRENT_BINARY_DIR}/MultiArrayConfig.cmake
#     ${CMAKE_CURRENT_BINARY_DIR}/MultiArrayConfigVersion.cmake
#     DESTINATION ${INSTALL_CONFIGDIR}
# )


