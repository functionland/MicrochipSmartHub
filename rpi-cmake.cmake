set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_SYSTEM_VERSION 1)

set(CROSS_PREFIX "arm-linux-gnueabihf")

# Define the cross compiler locations
if(NOT TOOLCHAIN_ROOT_DIR) # If the toolchain directory is not passed via command line
    set(TOOLCHAIN_ROOT_DIR /usr/${CROSS_PREFIX})
endif()

# Check if the cross compiler is installed
if(NOT EXISTS "${TOOLCHAIN_ROOT_DIR}")
    message(FATAL_ERROR "\
 ARM toolchain is not installed.\n\
 On debian based systems it can be installed by the command:\n\
     sudo apt install g{cc,++}-arm-linux-gnueabihf\n\
 Or you can specify its location by specifying it via -DTOOLCHAIN_ROOT_DIR=")
endif()

include_directories(include ${TOOLCHAIN_ROOT_DIR}/local/include)

# Define our compilers
set(CMAKE_C_COMPILER ${CROSS_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_PREFIX}-g++)

# Define the sysroot
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_ROOT_DIR})
message(STATUS "Searching for ARM toolchain in: ${CMAKE_FIND_ROOT_PATH}")

# Use our definitions for compiler tools
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search for libraries and headers in the target directories only
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

