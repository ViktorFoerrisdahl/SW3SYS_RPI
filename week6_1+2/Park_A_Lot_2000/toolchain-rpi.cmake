set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(TOOLCHAIN_DIR "C:/Program Files (x86)/Arm GNU Toolchain aarch64-none-linux-gnu/14.2 rel1/bin")
set(CMAKE_C_COMPILER   "${TOOLCHAIN_DIR}/aarch64-none-linux-gnu-gcc.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_DIR}/aarch64-none-linux-gnu-g++.exe")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)