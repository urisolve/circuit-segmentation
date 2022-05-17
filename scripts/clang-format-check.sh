#!/usr/bin/bash

# Checks format using clang-format.
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

echo "Clang-format checking..."
find $src_folder -type f -iname "*.h" -o -iname "*.cpp" | xargs clang-format --dry-run -Werror --style=file

echo "Clang-format check end"
