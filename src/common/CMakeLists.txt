# ----------------------------------------------------------------------------
# Project setup
project(Common)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    RandomNumberGen.h
)
set(Sources
    RandomNumberGen.cpp
)

# ----------------------------------------------------------------------------
# Library
add_library(${PROJECT_NAME}
    STATIC ${Headers} ${Sources}
)
add_library(CircuitSegmentation::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
