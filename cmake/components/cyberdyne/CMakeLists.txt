cmake_minimum_required(VERSION 3.0)
set(PROJECT "cyberdyne")
project(${PROJECT} CXX C)
set(CMAKE_CXX_STANDARD 17) # Needed by ruckig.

# Set variables.
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Set includepaths.
include_directories(${SRC_DIR})
include_directories(${SRC_DIR}/../../../src/rtapi)
include_directories(${SRC_DIR}/../../../src/hal)
include_directories(${SRC_DIR}/../../vendor/ruckig_dev/include)
include_directories(${SRC_DIR}/../../vendor/ruckig_dev/include/ruckig)

# Set prefix.
set(CMAKE_SHARED_LIBRARY_PREFIX "")

# Set compile flags.
SET(GCC_COVERAGE_COMPILE_FLAGS "-I. -I${RTAPI_DIR} -I${HAL_DIR} \
    -DUSPACE -fno-fast-math -fno-unsafe-math-optimizations -DRTAPI -D_GNU_SOURCE
    -Drealtime -D__MODULE__ -DSIM -fPIC -Os")
add_definitions(-DRTAPI -DUSPACE -D_GNU_SOURCE -D__MODULE__ )

ADD_LIBRARY(${PROJECT} SHARED
    ${SRC_DIR}/cyberdyne.c
    ${SRC_DIR}/../../vendor/ruckig_dev/include/ruckig/ruckig_dev_format.h
    ${SRC_DIR}/../../vendor/ruckig_dev/include/ruckig/ruckig_dev_interface.h
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/ruckig_dev_interface.cpp
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/brake.cpp
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/position_first_step1.cpp
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/position_first_step2.cpp
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/position_second_step1.cpp
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/position_second_step2.cpp
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/position_third_step1.cpp
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/position_third_step2.cpp
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/velocity_second_step1.cpp
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/velocity_second_step2.cpp
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/velocity_third_step1.cpp
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/velocity_third_step2.cpp
    )

# Set include paths for libraries to link.
add_library(linuxcnchal SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(linuxcnchal PROPERTIES
    IMPORTED_LOCATION "${SRC_DIR}/../../build/liblinuxcnchal/liblinuxcnchal.so"
    INTERFACE_INCLUDE_DIRECTORIES "${SRC_DIR}/../../liblinuxcnchal/"
    )

# Link to libraries.
target_link_libraries(${PROJECT_NAME} PRIVATE linuxcnchal)

# Set custom output name :
# set_property(TARGET  ${PROJECT} PROPERTY OUTPUT_NAME thename)

# Make install
install(TARGETS ${PROJECT} DESTINATION ${BUILD_DIR}/../../../../rtlib)

# Notes.
