cmake_minimum_required(VERSION 3.0)
set(PROJECT "halcmd")
project(${PROJECT} )

# Set variables.
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Set includepaths.
include_directories(${SRC_DIR})
include_directories(${SRC_DIR}/../../src)
include_directories(${SRC_DIR}/../../src/emc)
include_directories(${SRC_DIR}/../../src/hal)
include_directories(${SRC_DIR}/../../src/hal/utils)
include_directories(${SRC_DIR}/../../src/rtapi)
include_directories(${SRC_DIR}/../../src/libnml/inifile)

# sudo apt-get install gcc-multilib g++-multilib

#include_directories(/usr/include/c++/12)
#include_directories(usr/include/bits/)
#include_directories(usr/include/)
# Set prefix.
# set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(SOURCES
    ${SRC_DIR}/../../src/hal/utils/halcmd.c
    ${SRC_DIR}/../../src/hal/utils/halcmd_commands.cc
    ${SRC_DIR}/../../src/hal/utils/halcmd_main.c
    ${SRC_DIR}/../../src/hal/utils/halcmd_completion.c
    ${SRC_DIR}/../../src/hal/hal_lib.c
    ${SRC_DIR}/../../src/rtapi/uspace_ulapi.c
    )
add_executable(${PROJECT_NAME} ${SOURCES})

add_definitions(-DSIM -DULAPI)

# Set include paths for libraries to link.
add_library(linuxcncini SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(linuxcncini PROPERTIES
  IMPORTED_LOCATION "${SRC_DIR}/../build/liblinuxcncini/liblinuxcncini.so"
  INTERFACE_INCLUDE_DIRECTORIES "${SRC_DIR}/../../include"
)

add_library(nml SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(nml PROPERTIES
  IMPORTED_LOCATION "${SRC_DIR}/../build/libnml/libnml.so"
  INTERFACE_INCLUDE_DIRECTORIES "${SRC_DIR}/../../include"
)

add_library(linuxcnchal SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(linuxcnchal PROPERTIES
  IMPORTED_LOCATION "${SRC_DIR}/../build/liblinuxcnchal/liblinuxcnchal.so"
  INTERFACE_INCLUDE_DIRECTORIES "${SRC_DIR}/../../include"
)

# Link to libraries.
target_link_libraries(${PROJECT_NAME} PRIVATE readline linuxcncini linuxcnchal tirpc gpiod rt)

# Set custom output name :
# set_property(TARGET  ${PROJECT} PROPERTY OUTPUT_NAME thename)

# Make install
install(TARGETS ${PROJECT} DESTINATION ${BUILD_DIR}/../../../bin)

# Notes.
