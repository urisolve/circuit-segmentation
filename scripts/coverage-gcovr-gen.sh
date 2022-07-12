#!/usr/bin/bash

# Generates gcov reports for code coverage, using gcovr tool.
# This scripts considers that the project was already compiled for coverage.
#
# Gcovr is available as a Python package that can be installed via pip:
# pip install gcovr
# More information about gcovr: https://github.com/gcovr/gcovr
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
    rm -rf $coverage_folder/*
else
    # Create folder
    echo "Creating folder for results..."
    mkdir -p $coverage_folder
fi

# Generate report
echo "Generating report..."
gcovr --html-details $coverage_folder/coverage.html --exclude-directories 'tests/*' --exclude-directories 'external/*' \
    -e '.*/src/main.cpp' -e '.*/src/application/Application.cpp' -r ..

# Print a tabular report to the output
gcovr -s --exclude-directories 'tests/*' --exclude-directories 'external/*' \
    -e '.*/src/main.cpp' -e '.*/src/application/Application.cpp' -r ..

echo "Coverage generation end"
