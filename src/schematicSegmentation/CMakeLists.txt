# ----------------------------------------------------------------------------
# Project setup
project(SchematicSegmentation)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    ComponentSegmentation.h
    ConnectionSegmentation.h
)
set(Sources
    ComponentSegmentation.cpp
    ConnectionSegmentation.cpp
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
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    PUBLIC ${OpenCV_INCLUDE_DIRS}
    PUBLIC ${CMAKE_SOURCE_DIR}/external/stduuid
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE CircuitSegmentation::ComputerVision
    PRIVATE CircuitSegmentation::Logger
    PRIVATE CircuitSegmentation::Circuit
)
