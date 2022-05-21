# ----------------------------------------------------------------------------
# Project setup
project(ImageProcessing)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    ImagePreprocessing.h
    ImageProcManager.h
    ImageReceiver.h
)
set(Sources
    ImagePreprocessing.cpp
    ImageProcManager.cpp
    ImageReceiver.cpp
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
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE CircuitSegmentation::ComputerVision
    PRIVATE CircuitSegmentation::Logger
)
