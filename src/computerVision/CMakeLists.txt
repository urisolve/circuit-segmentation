# ----------------------------------------------------------------------------
# Project setup
project(ComputerVision)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    OpenCvWrapper.h
)
set(Sources
    OpenCvWrapper.cpp
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
    PRIVATE ${OpenCV_LIBS}
)
