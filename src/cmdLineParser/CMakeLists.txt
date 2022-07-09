# ----------------------------------------------------------------------------
# Project setup
project(CmdLineParser)

# ----------------------------------------------------------------------------
# Source files
set(Headers
    CmdLineParser.h
)
set(Sources
    CmdLineParser.cpp
)

# ----------------------------------------------------------------------------
# Library
add_library(${PROJECT_NAME}
    STATIC ${Headers} ${Sources}
)
add_library(CircuitSegmentation::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
