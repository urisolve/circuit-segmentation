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
if [ "$#" -ne 1 ]
then
    echo "Usage:"
    echo "$0 <build_folder>"
    echo
    exit -1
fi

# Build folder
build_folder=$1

# Run test command
echo "Running ctest..."
cd $build_folder
ctest

# Generate report
echo "Generating report..."
mkdir -p coverage
gcovr --html-details coverage/coverage.html --exclude-directories='tests.*' --exclude='.*/src/main.cpp' --exclude='.*/src/application/Application.cpp' -r ..

# Print a tabular report on the console
gcovr --exclude-directories='tests.*' --exclude='.*/src/main.cpp' --exclude='.*/src/application/Application.cpp' -r ..

echo "Coverage generation end"
