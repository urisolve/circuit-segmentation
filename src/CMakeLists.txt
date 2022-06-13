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
add_subdirectory(circuit)
add_subdirectory(cmdLineParser)
add_subdirectory(common)
add_subdirectory(computerVision)
add_subdirectory(imageProcessing)
add_subdirectory(logging)

target_link_libraries(${PROJECT_NAME}
    PRIVATE CircuitSegmentation::Application
)
