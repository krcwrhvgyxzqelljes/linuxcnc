cmake_minimum_required(VERSION 3.0)
set(PROJECT "motmod_T800")
set(CMAKE_CXX_STANDARD 20) # Needed by ruckig.
project(${PROJECT} CXX C)

# Set variables.
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Set includepaths.
include_directories(${SRC_DIR}/../../../src)
include_directories(${SRC_DIR}/../../../src/rtapi)
include_directories(${SRC_DIR}/../../../src/hal)
include_directories(${SRC_DIR}/../../../src/emc/motion)
include_directories(${SRC_DIR}/../../../src/emc/nml_intf)
include_directories(${SRC_DIR}/../../../src/emc/kinematics)
include_directories(${SRC_DIR}/../../../src/emc/tp)
include_directories(${SRC_DIR}/../../../src/libnml/posemath)
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
    # Core file.
    ${SRC_DIR}/../../../src/emc/motion/simple_tp_T800.c
    # Helpers.
    ${SRC_DIR}/../../../src/emc/kinematics/cubic.c
    ${SRC_DIR}/../../../src/emc/motion/axis.c
    # In motion_T800.c edit your component name.
    ${SRC_DIR}/../../../src/emc/motion/motion_T800.c
    ${SRC_DIR}/../../../src/emc/motion/command.c
    ${SRC_DIR}/../../../src/emc/motion/control.c
    ${SRC_DIR}/../../../src/emc/motion/simple_tp.h
    ${SRC_DIR}/../../../src/emc/motion/emcmotutil.c
    ${SRC_DIR}/../../../src/emc/motion/stashf.c
    ${SRC_DIR}/../../../src/emc/motion/dbuf.c
    # Ruckig-dev
    ${SRC_DIR}/../../vendor/ruckig_dev/include/ruckig/ruckig_dev_format.h
    ${SRC_DIR}/../../vendor/ruckig_dev/include/ruckig/ruckig_dev_online_interface.h
    ${SRC_DIR}/../../vendor/ruckig_dev/src/ruckig/ruckig_dev_online_interface.cpp
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

# Set path to eigen3, for install $ sudo apt-get install libeigen3-dev
include_directories("/usr/include/eigen3/Eigen")

# Set include paths for libraries to link.
add_library(linuxcnchal SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(linuxcnchal PROPERTIES
    IMPORTED_LOCATION "${SRC_DIR}/../../build/liblinuxcnchal/liblinuxcnchal.so"
    INTERFACE_INCLUDE_DIRECTORIES "${SRC_DIR}/../../../include"
    )

# Link to libraries.
target_link_libraries(${PROJECT_NAME} PRIVATE linuxcnchal)

# Set custom output name :
# set_property(TARGET  ${PROJECT} PROPERTY OUTPUT_NAME thename)

# Make install
install(TARGETS ${PROJECT} DESTINATION ${BUILD_DIR}/../../../../rtlib)

# Notes.


