# ----------------------------------------------------------------------------
# Project setup
project(UtApplication)

# ----------------------------------------------------------------------------
# Test
enable_testing()

# ----------------------------------------------------------------------------
# Source files
set(Sources
    ut_CommandLineParser.cpp
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

target_link_libraries(${PROJECT_NAME}
    PRIVATE GTest::gtest_main
    PRIVATE CircuitSegmentation::Application
)
