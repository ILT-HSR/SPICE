project(SPICE CXX C)
cmake_minimum_required(VERSION 3.0.0)
set (CMAKE_VERBOSE_MAKEFILE on)

if (NOT MINGW)
    message( FATAL_ERROR "SPICE CMake build has been tested only with MINGW yet. CMake will exit." )
endif ()

# Append our module directory to CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Windows DLLs are "runtime" for CMake. Output them to "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#################################################################################
# Setup C/C++ compiler options
#################################################################################
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Choose the type of build, options are: None Debug Release" FORCE)
endif()
message(STATUS "Setting SPICE build type - ${CMAKE_BUILD_TYPE}")

if (CMAKE_BUILD_TYPE STREQUAL "")
    set( CMAKE_BUILD_TYPE "RelWithDebInfo" )
endif ()

# For Debug build types, append a "d" to the library names.
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Set debug library postfix" FORCE)

# Uncomment from next two lines to force statitc or dynamic library, default is autodetection
if(SPICE_STATIC)
    set( LIB_MODE STATIC )
    message(STATUS "Building static libraries")
else(SPICE_STATIC)
    set( LIB_MODE SHARED )
    message(STATUS "Building dynamic libraries")
endif(SPICE_STATIC)

option(SPICE_STATIC
  "Set to OFF|ON (default is OFF) to control build of POCO as STATIC library" OFF)

add_subdirectory(SPICE_BIG)
add_subdirectory(SPICE_Core)
add_subdirectory(SPICE_ES_POCO)
add_subdirectory(SPICE_XML_POCO)

