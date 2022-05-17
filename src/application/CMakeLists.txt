# ----------------------------------------------------------------------------
# Project setup
project(Application)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    Application.h
    CommandLineParser.h
)
set(Sources
    Application.cpp
    CommandLineParser.cpp
)

# ----------------------------------------------------------------------------
# Library
add_library(${PROJECT_NAME}
    STATIC ${Headers} ${Sources}
)
add_library(CircuitSegmentation::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ----------------------------------------------------------------------------
# Build
target_link_libraries(${PROJECT_NAME}
    PRIVATE CircuitSegmentation::ImageProcessing
    PRIVATE CircuitSegmentation::Logger
    PRIVATE CircuitSegmentation::Utils
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
