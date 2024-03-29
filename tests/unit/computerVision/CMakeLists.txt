# ----------------------------------------------------------------------------
# Project setup
project(UtOpenCvWrapper)

# ----------------------------------------------------------------------------
# Test
enable_testing()

# ----------------------------------------------------------------------------
# Source files
set(Sources
    ut_OpenCvWrapper.cpp
)

# ----------------------------------------------------------------------------
# Executables
add_executable(${PROJECT_NAME}
    ${Sources}
)

# ----------------------------------------------------------------------------
# Tests
gtest_discover_tests(${PROJECT_NAME})

# ----------------------------------------------------------------------------
# Build

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC TESTS_DATA_PATH="${CMAKE_SOURCE_DIR}/tests/data/"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE GTest::gtest_main
    PRIVATE CircuitSegmentation::ComputerVision
)
