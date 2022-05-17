# ----------------------------------------------------------------------------
# Project setup
project(Logger)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    Logger.h
)
set(Sources
    Logger.cpp
)

# ----------------------------------------------------------------------------
# Library
add_library(${PROJECT_NAME}
    STATIC ${Headers} ${Sources}
)
add_library(CircuitSegmentation::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
