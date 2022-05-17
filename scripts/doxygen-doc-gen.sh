#!/usr/bin/bash

# Creates documentation using doxygen.
# 
# Usage:
# ./<script>.sh <doxyfile>

# Check script usage
if [ "$#" -ne 1 ]
then
    echo "Usage:"
    echo "$0 <doxyfile>"
    echo
    exit -1
fi

# Doxygen file
doxyfile=$1

echo "Generating doxygen documentation..."
doxygen $doxyfile

echo "Doxygen documentation end"
