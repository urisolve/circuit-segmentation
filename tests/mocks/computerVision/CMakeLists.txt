# ----------------------------------------------------------------------------
# Project setup
project(MockOpenCvWrapper)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    MockOpenCvWrapper.h
)

# ----------------------------------------------------------------------------
# Library
add_library(${PROJECT_NAME}
    STATIC ${Headers}
)
add_library(CircuitSegmentation::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ----------------------------------------------------------------------------
# Build

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE GTest::gmock
    PRIVATE CircuitSegmentation::ComputerVision
)
