# ----------------------------------------------------------------------------
# Project setup

cmake_minimum_required(VERSION 3.16)

project(CircuitSegmentation
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

# ----------------------------------------------------------------------------
# CMake options

# CMAKE_BUILD_TYPE
# Specifies the build type on single-configuration generators (e.g. Makefile generators or Ninja). Typical values
# include Debug, Release, RelWithDebInfo and MinSizeRel, but custom build types can also be defined.
# This variable is initialized by the first project() or enable_language() command called in a project when a new build
# tree is first created. If the CMAKE_BUILD_TYPE environment variable is set, its value is used. Otherwise, a
# toolchain-specific default is chosen when a language is enabled. The default value is often an empty string, but this
# is usually not desirable and one of the other standard build types is usually more appropriate.
# See https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html.
# Set default build type if build type not set.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# CMAKE_CONFIGURATION_TYPES
# Specifies the available build types (configurations) on multi-config generators (e.g. Visual Studio, Xcode, or Ninja
# Multi-Config). Typical values include Debug, Release, RelWithDebInfo and MinSizeRel, but custom build types can also
# be defined.
# This variable is initialized by the first project() or enable_language() command called in a project when a new build
# tree is first created. If the CMAKE_CONFIGURATION_TYPES environment variable is set, its value is used. Otherwise, the
# default value is generator-specific.
# See https://cmake.org/cmake/help/latest/variable/CMAKE_CONFIGURATION_TYPES.html.
# Set default configuration type if configuration type not set.
if (NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES Debug)
endif()

# Option to build unit tests
option(BUILD_TESTS "Builds unit tests" OFF)
# Option to build with code coverage
option(BUILD_COVERAGE "Builds with code coverage" OFF)

# ----------------------------------------------------------------------------
# Dependencies

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Fetch dependencies
include(FetchDependencies)

# OpenCV
find_package(OpenCV REQUIRED)

# ----------------------------------------------------------------------------
# Compile definitions

# Set build debug definition
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(BUILD_DEBUG)
endif()

# Set build tests definition
if (BUILD_TESTS)
    add_compile_definitions(BUILD_TESTS)
endif()

# ----------------------------------------------------------------------------
# Test
if (BUILD_TESTS)
    include(CTest)
    include(GoogleTest)
    enable_testing()
endif()

# ----------------------------------------------------------------------------
# Build

# Set compile options for GCC
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Werror"
    )
endif()

# Set compile options for MSVC
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} /WX"
    )
endif()

# Code coverage
if(BUILD_COVERAGE)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} --coverage -g -O0"
        )
    endif()
endif()

# Subdirectories
add_subdirectory(src)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

# ----------------------------------------------------------------------------
# OpenCV variables are set with find_package, the full list with descriptions is in the OpenCVConfig.cmake file
message(STATUS)
message(STATUS "OpenCV configuration:")
message(STATUS "- OpenCV_VERSION = ${OpenCV_VERSION}")
message(STATUS "- OpenCV_DIR = ${OpenCV_DIR}")
message(STATUS "- OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "- OpenCV_LIBS = ${OpenCV_LIBS}")

# ----------------------------------------------------------------------------
# Status
message(STATUS)
message(STATUS "${PROJECT_NAME} configuration:")
message(STATUS "- CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "- CMAKE_CONFIGURATION_TYPES = ${CMAKE_CONFIGURATION_TYPES}")
message(STATUS "- CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "- CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")
message(STATUS "- CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message(STATUS "- BUILD_TESTS = ${BUILD_TESTS}")
message(STATUS "- BUILD_COVERAGE = ${BUILD_COVERAGE}")
message(STATUS)
