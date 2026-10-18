cmake_minimum_required(VERSION 3.15...4.10)

project(Park_A_Lot_2000 VERSION 1.0
                DESCRIPTION "HandIN 1 SYS"
                LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall")

set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_CXX_COMPILER g++) //Ved cross compile skal vi ikke angive i cmake hvilket system vi skal compile til. Dette gøres automatisk når vi bygger vores project.
# Når vi bygger til windows: 
# cmake -B build-windows -G Ninja .
# cmake --build build-windows

# Når vi bygger til pi:
# cmake -B build-rpi -G Ninja --toolchain toolchain-rpi.cmake .
# cmake --build build-rpi

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin")

add_subdirectory(src)

message(STATUS "DEBUG: ${PROJECT_SOURCE_DIR}")