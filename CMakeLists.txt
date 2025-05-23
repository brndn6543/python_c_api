cmake_minimum_required(VERSION 3.31)
project(python_c_api C)

set(CMAKE_C_STANDARD 11)

# Disable Python debug libs (usually not available)
set(Python3_USE_DEBUG_LIBS OFF)

# Find Python from virtual environment or system
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Include Python headers
include_directories(${Python3_INCLUDE_DIRS})

# Define your executable
add_executable(python_c_api main.c)

# Link Python libraries (Release versions explicitly)
target_link_libraries(python_c_api ${Python3_LIBRARIES})
