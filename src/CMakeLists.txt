# ----------------------------------------------------------------------------
# Project setup
project(CircuitSegmentation)

# ----------------------------------------------------------------------------
# Source files
set(Sources
    main.cpp
)

# ----------------------------------------------------------------------------
# Executables
add_executable(${PROJECT_NAME}
    ${Sources}
)

# ----------------------------------------------------------------------------
# Build

# Subdirectories
add_subdirectory(application)
add_subdirectory(imageProcessing)
add_subdirectory(logging)
add_subdirectory(utils)

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE CircuitSegmentation::Application
)
