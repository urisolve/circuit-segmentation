#!/usr/bin/bash

# Analyses statically using clang-tidy.
# This scripts uses CMake to set up a compile command database of the project, and must be run from the project root
# directory.
#
# Usage:
# ./<script>.sh

# Build folder (created if not exist)
build_folder=build-clang-tidy
# Report file
report_file=$build_folder/report.txt

echo "Clang-tidy checking..."

# Cmake
cmake -S . -B $build_folder -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Clang-tidy
files=$(find src -type f -iname "*.h" -o -iname "*.cpp")
clang-tidy $files -p $build_folder -header-filter=src/* > $report_file

echo "Clang-tidy check end"
