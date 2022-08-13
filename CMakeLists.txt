# ----------------------------------------------------------------------------
# Project setup

cmake_minimum_required(VERSION 3.22)

project(CircuitSegmentation
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

# ----------------------------------------------------------------------------
# CMake options

# Set default build type if build type not set
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
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

# Set compile options for g++
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
message(STATUS "- CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "- CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")
message(STATUS "- CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message(STATUS "- BUILD_TESTS = ${BUILD_TESTS}")
message(STATUS "- BUILD_COVERAGE = ${BUILD_COVERAGE}")
message(STATUS)
