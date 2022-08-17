# ----------------------------------------------------------------------------
# Project setup
project(ImageProcessing)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    ImagePreprocessing.h
    ImageProcManager.h
    ImageReceiver.h
    ImageSegmentation.h
)
set(Sources
    ImagePreprocessing.cpp
    ImageProcManager.cpp
    ImageReceiver.cpp
    ImageSegmentation.cpp
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
    PRIVATE CircuitSegmentation::Logger
    PRIVATE CircuitSegmentation::SchematicSegmentation
    PUBLIC stduuid
    PUBLIC nlohmann_json::nlohmann_json
)
