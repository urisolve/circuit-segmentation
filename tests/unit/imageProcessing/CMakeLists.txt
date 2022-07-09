# ----------------------------------------------------------------------------
# Project setup
project(UtImageProcessing)

# ----------------------------------------------------------------------------
# Test
enable_testing()

# ----------------------------------------------------------------------------
# Source files
set(Sources
    ut_ImagePreprocessing.cpp
    ut_ImageReceiver.cpp
    ut_ImageSegmentation.cpp
    ut_ImageUtils.cpp
)

# ----------------------------------------------------------------------------
# Executables
add_executable(${PROJECT_NAME}
    ${Sources}
)

# ----------------------------------------------------------------------------
# Tests
# add_test(${PROJECT_NAME}
#     ${PROJECT_NAME}
# )
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
    PRIVATE CircuitSegmentation::ImageProcessing
    PRIVATE CircuitSegmentation::Logger
)
