cmake_minimum_required(VERSION 3.0)
set(PROJECT "linuxcnc_module_helper")
project(${PROJECT} C)

# Set variables.
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Set includepaths.
include_directories(${SRC_DIR})
include_directories(${SRC_DIR}/../../src)
include_directories(${SRC_DIR}/../../src/emc)
include_directories(${SRC_DIR}/../../src/hal)
include_directories(${SRC_DIR}/../../src/rtapi)

# Set prefix.
set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(SOURCES
    ${SRC_DIR}/../../src/module_helper/module_helper.c
    )
add_executable(${PROJECT_NAME} ${SOURCES})

add_definitions(-DSIM -DRTAPI)

# Set include paths for libraries to link.
target_include_directories(${PROJECT_NAME} PRIVATE ${BUILD_DIR})

# Link to libraries.
target_link_libraries(${PROJECT_NAME} PRIVATE -Wall -Werror)

# Set custom output name :
# set_property(TARGET  ${PROJECT} PROPERTY OUTPUT_NAME thename)

# Make install
install(TARGETS ${PROJECT} DESTINATION ${BUILD_DIR}/../../../bin)

# Notes.
