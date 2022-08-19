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

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE CircuitSegmentation::ImageProcessing
    PRIVATE CircuitSegmentation::CmdLineParser
    PRIVATE CircuitSegmentation::Logger
    PUBLIC nlohmann_json::nlohmann_json
)
