# ----------------------------------------------------------------------------
# Project setup
project(Circuit)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    Component.h
    Connection.h
    Id.h
    Label.h
    Node.h
    Port.h
    Position.h
    Schematic.h
)
set(Sources
    Component.cpp
    Connection.cpp
    Label.cpp
    Node.cpp
    Port.cpp
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
    PRIVATE CircuitSegmentation::Common
    PUBLIC stduuid
)
