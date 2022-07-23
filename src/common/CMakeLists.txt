# ----------------------------------------------------------------------------
# Project setup
project(Common)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    RandomNumberGen.h
    UuidGen.h
)
set(Sources
    RandomNumberGen.cpp
    UuidGen.cpp
)

# ----------------------------------------------------------------------------
# Library
add_library(${PROJECT_NAME}
    STATIC ${Headers} ${Sources}
)
add_library(CircuitSegmentation::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ----------------------------------------------------------------------------
# Build

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/external/stduuid
)
