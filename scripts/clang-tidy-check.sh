#!/usr/bin/bash

# Analyses statically using clang-tidy.
# 
# Usage:
# ./<script>.sh <source_code_folder>

# Check script usage
if [ "$#" -ne 1 ]
then
    echo "Usage:"
    echo "$0 <source_code_folder>"
    echo
    exit -1
fi

# Source code folder
src_folder=$1

echo "Clang-tidy checking..."
files=$(find $src_folder -type f -iname "*.h" -o -iname "*.cpp")
clang-tidy $files

echo "Clang-tidy check end"
