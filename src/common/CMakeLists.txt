# ----------------------------------------------------------------------------
# Project setup
project(Common)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    UuidGen.h
)
set(Sources
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
