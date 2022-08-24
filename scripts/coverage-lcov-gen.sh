#!/usr/bin/bash

# Generates gcov reports for code coverage, using LCOV tool.
# This scripts considers that the project was already compiled for coverage.
# 
# Usage:
# ./<script>.sh <build_folder>
#
# Example:
# ./<script>.sh build

# Check script usage
if [ "$#" -ne 1 ]; then
    echo "Usage:"
    echo "$0 <build_folder>"
    echo
    exit -1
fi

# Build folder
build_folder=$1
# Code coverage folder
coverage_folder=coverage

# Run test command
echo "Running ctest..."
cd $build_folder
ctest

# Folder for results
if [ -d "$coverage_folder" ]; then
    # Delete previous results
    echo "Deleting previous results..."
    rm -rf ./$coverage_folder/*
else
    # Create folder
    echo "Creating folder for results..."
    mkdir -p $coverage_folder
fi

# Generate report
echo "Generating report..."
lcov -c -d ./src -o coverage.info
lcov -r coverage.info */_deps/\* /usr/include/\* -o coverage_filtered.info
genhtml coverage_filtered.info -o $coverage_folder

echo "Coverage generation end"
