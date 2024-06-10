cmake_minimum_required(VERSION 3.0)
set(PROJECT "tpmod_scurve_skynet")
set(CMAKE_CXX_STANDARD 20) # Needed by ruckig.
project(${PROJECT} CXX C)

# Set variables.
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(RTLINK "liblinuxcnchal")

# Set includepaths.
include_directories(${SRC_DIR})

include_directories(${SRC_DIR}/../../../src/rtapi)
include_directories(${SRC_DIR}/../../../src/hal)
include_directories(${SRC_DIR}/../../../src/libnml/posemath)
include_directories(${SRC_DIR}/../../../src/emc/tp)
include_directories(${SRC_DIR}/../../../src/emc/motion)
include_directories(${SRC_DIR}/../../../src/emc/kinematics)
include_directories(${SRC_DIR}/../../../src/emc/nml_intf)
include_directories(${SRC_DIR}/../tpmod_scurve_src)
include_directories(${SRC_DIR}/../../vendor/ruckig)
include_directories(${SRC_DIR}/../../vendor/angular_motion)
include_directories(${SRC_DIR}/../../vendor/scurve/scurve_engine)

# Set path to eigen3, for install $ sudo apt-get install libeigen3-dev
include_directories("/usr/include/eigen3/Eigen")

# Set prefix.
set(CMAKE_SHARED_LIBRARY_PREFIX "")

# Set compile flags.
SET(GCC_COVERAGE_COMPILE_FLAGS "-I. -I${RTAPI_DIR} -I${HAL_DIR} \
    -DUSPACE -fno-fast-math -fno-unsafe-math-optimizations -DRTAPI -D_GNU_SOURCE
    -Drealtime -D__MODULE__ -DSIM -fPIC -Os")
add_definitions(-DRTAPI -DUSPACE -D_GNU_SOURCE -D__MODULE__ )

ADD_LIBRARY(${PROJECT} SHARED
    ${SRC_DIR}/../../../src/emc/tp/tc.h
    ${SRC_DIR}/../../../src/emc/tp/tc.c
    ${SRC_DIR}/../../../src/emc/tp/tp.h
    ${SRC_DIR}/../../../src/emc/tp/tp_types.h
    ${SRC_DIR}/../../../src/emc/tp/spherical_arc.c
    ${SRC_DIR}/../../../src/emc/tp/blendmath.c
    ${SRC_DIR}/../../../src/emc/nml_intf/emcpose.c
    ${SRC_DIR}/../../../src/libnml/posemath/_posemath.c
    ${SRC_DIR}/../../../src/libnml/posemath/sincos.c

    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_scurve_skynet.c

    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_corners.h
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_corners.cpp
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_conversion.h
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_conversion.c
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_vector.h
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_vector.cpp
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_arcs.h
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_arcs.cpp
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_lines.h
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_lines.cpp
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/scurve_engine.h
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/scurve_engine.cpp
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/scurve_reset.h
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/scurve_struct.h
    ${SRC_DIR}/../../vendor/scurve/scurve_engine/tp_scurve.h

    ${SRC_DIR}/../../vendor/angular_motion/angular_motion.h
    ${SRC_DIR}/../../vendor/angular_motion/angular_motion.cpp

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

