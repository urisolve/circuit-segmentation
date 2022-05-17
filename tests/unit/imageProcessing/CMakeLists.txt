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
    ut_ImageProcUtils.cpp
    ut_ImageReceiver.cpp
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
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

# ----------------------------------------------------------------------------
# Build
target_link_libraries(${PROJECT_NAME}
    PRIVATE gtest
    PRIVATE gtest_main
    PRIVATE gmock
    PRIVATE pthread
    PRIVATE CircuitSegmentation::ImageProcessing
    PRIVATE CircuitSegmentation::MockLogger
)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    PRIVATE ${CMAKE_SOURCE_DIR}/tests/mocks
)
target_compile_definitions(${PROJECT_NAME}
    PUBLIC TESTS_DATA_PATH="${CMAKE_SOURCE_DIR}/tests/data/"
)
