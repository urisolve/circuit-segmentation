# ----------------------------------------------------------------------------
# Project setup
project(UtSchematicSegmentation)

# ----------------------------------------------------------------------------
# Test
enable_testing()

# ----------------------------------------------------------------------------
# Source files
set(Sources
    ut_ComponentDetection.cpp
    ut_ConnectionDetection.cpp
    ut_SchematicSegmentation.cpp
    ut_SegmentationUtils.cpp
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
    PRIVATE ${CMAKE_SOURCE_DIR}/tests
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE GTest::gtest_main
    PRIVATE GTest::gmock
    PRIVATE CircuitSegmentation::SchematicSegmentation
    PRIVATE CircuitSegmentation::Logger
)
