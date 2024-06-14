cmake_minimum_required(VERSION 3.0)
set(PROJECT "librs274")
project(${PROJECT} VERSION 0 LANGUAGES CXX C)

# Set variables.
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Set includepaths.
include_directories(${SRC_DIR})
include_directories(${SRC_DIR}/../../src)
include_directories(${SRC_DIR}/../../src/rtapi)
include_directories(${SRC_DIR}/../../src/emc)
include_directories(${SRC_DIR}/../../src/emc/motion)
include_directories(${SRC_DIR}/../../src/emc/nml_intf)
include_directories(${SRC_DIR}/../../src/emc/rs274ngc)
include_directories(${SRC_DIR}/../../src/emc/pythonplugin)
include_directories(${SRC_DIR}/../../src/libnml/posemath)
include_directories(${SRC_DIR}/../../src/libnml/nml)
include_directories(${SRC_DIR}/../../src/libnml/rcs)
include_directories(${SRC_DIR}/../../src/libnml/cms)
include_directories(${Python3_INCLUDE_DIRS})
include_directories(/usr/include/boost)

# Set prefix.
set(CMAKE_SHARED_LIBRARY_PREFIX "")

ADD_LIBRARY(${PROJECT} SHARED
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_arc.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_array_types.hh # must be installed in /include
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_array.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_base.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_check.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_convert.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_queue.hh # must be installed in /include
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_queue.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_cycles.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_execute.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_find.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_internal.hh # must be installed in /include
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_internal.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_inverse.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_read.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_write.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_o_word.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_g7x.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/modal_state.hh # must be installed in /include
    ${SRC_DIR}/../../src/emc/rs274ngc/modal_state.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/nurbs_additional_functions.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_namedparams.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_python.hh # must be installed in /include
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_python.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_remap.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_setup.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/canonmodule.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/pyparamclass.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/pyemctypes.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/pyinterp1.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/pyblock.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/pyarrays.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interpmodule.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/rs274ngc_pre.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_inspection.hh # must be installed in /include
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_inspection.cc
    ${SRC_DIR}/../../src/emc/rs274ngc/interp_parameter_def.hh # must be installed in /include
    ${SRC_DIR}/../../src/emc/rs274ngc/rs274ngc_interp.hh # must be installed in /include
    ${SRC_DIR}/../../src/emc/rs274ngc/rs274ngc.hh # must be installed in /include
    ${SRC_DIR}/../../src/emc/nml_intf/canon.hh # must be installed in /include

    )

if (BUILD_USPACE)
    add_definitions(-DULAPI)
else ()
    add_definitions(-DRTAPI)
endif ()


# Set include paths for libraries to link.
add_library(linuxcnchal SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(linuxcnchal PROPERTIES
    IMPORTED_LOCATION "${SRC_DIR}/../build/liblinuxcnchal/liblinuxcnchal.so"
    INTERFACE_INCLUDE_DIRECTORIES "${SRC_DIR}/../../src/hal"
    )

add_library(linuxcncini SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(linuxcncini PROPERTIES
    IMPORTED_LOCATION "${SRC_DIR}/../build/liblinuxcncini/liblinuxcncini.so"
    INTERFACE_INCLUDE_DIRECTORIES "${SRC_DIR}/../../src/libnml/inifile"
    )

add_library(tooldata SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(tooldata PROPERTIES
    IMPORTED_LOCATION "${SRC_DIR}/../build/libtooldata/libtooldata.so"
    INTERFACE_INCLUDE_DIRECTORIES "${SRC_DIR}/../../src/emc/tooldata"
    )

add_library(pyplugin SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(pyplugin PROPERTIES
    IMPORTED_LOCATION "${SRC_DIR}/../build/libpyplugin/libpyplugin.so"
    INTERFACE_INCLUDE_DIRECTORIES "${SRC_DIR}/../../src/emc/pythonplugin"
    )

# Link to libraries.
target_link_libraries(${PROJECT_NAME} PRIVATE pyplugin linuxcncini linuxcnchal tooldata
    ${PYTHON_LIB}
    )

# Add verion nr. .so.0.0.0
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# Set custom output name :
# set_property(TARGET  ${PROJECT} PROPERTY OUTPUT_NAME thename)

# Make install
#install(TARGETS ${PROJECT} DESTINATION ${BUILD_DIR}/../../../lib)
#install(FILES ${SRC_DIR}/../../src/emc/nml_intf/canon.hh DESTINATION ${BUILD_DIR}/../../../include)
#install(FILES ${SRC_DIR}/../../src/emc/rs274ngc/rs274ngc_interp.hh DESTINATION ${BUILD_DIR}/../../../include)
#install(FILES ${SRC_DIR}/../../src/emc/rs274ngc/interp_parameter_def.hh DESTINATION ${BUILD_DIR}/../../../include)
#install(FILES ${SRC_DIR}/../../src/emc/rs274ngc/interp_queue.hh DESTINATION ${BUILD_DIR}/../../../include)
#install(FILES ${SRC_DIR}/../../src/emc/rs274ngc/interp_internal.hh DESTINATION ${BUILD_DIR}/../../../include)
#install(FILES ${SRC_DIR}/../../src/emc/rs274ngc/rs274ngc.hh DESTINATION ${BUILD_DIR}/../../../include)
#install(FILES ${SRC_DIR}/../../src/emc/rs274ngc/interp_python.hh  DESTINATION ${BUILD_DIR}/../../../include)
#install(FILES ${SRC_DIR}/../../src/emc/rs274ngc/interp_inspection.hh DESTINATION ${BUILD_DIR}/../../../include)
#install(FILES ${SRC_DIR}/../../src/emc/rs274ngc/modal_state.hh  DESTINATION ${BUILD_DIR}/../../../include)
#install(FILES ${SRC_DIR}/../../src/emc/rs274ngc/interp_array_types.hh DESTINATION ${BUILD_DIR}/../../../include)











