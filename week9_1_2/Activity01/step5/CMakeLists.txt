cmake_minimum_required(VERSION 3.10)

project(test_message LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add include path
include_directories(${CMAKE_SOURCE_DIR}/include)

# Create executable from source files
add_executable(test_message
    src/test.cpp
)
