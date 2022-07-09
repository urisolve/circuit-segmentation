# ----------------------------------------------------------------------------
# Project setup
project(Circuit)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    Component.h
    Node.h
)
set(Sources
    Component.cpp
    Node.cpp
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
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE CircuitSegmentation::ComputerVision
)
