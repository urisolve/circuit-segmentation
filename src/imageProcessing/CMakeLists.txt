# ----------------------------------------------------------------------------
# Project setup
project(ImageProcessing)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    ImagePreprocessing.h
    ImageProcManager.h
    ImageProcUtils.h
    ImageReceiver.h
)
set(Sources
    ImagePreprocessing.cpp
    ImageProcManager.cpp
    ImageProcUtils.cpp
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
target_link_libraries(${PROJECT_NAME}
    PRIVATE CircuitSegmentation::Logger
    PRIVATE ${OpenCV_LIBS}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
