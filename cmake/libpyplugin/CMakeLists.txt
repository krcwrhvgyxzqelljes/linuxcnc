cmake_minimum_required(VERSION 3.0)
set(PROJECT "libpyplugin")
project(${PROJECT} VERSION 0 LANGUAGES CXX)

# Set variables.
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Set includepaths.
include_directories(${SRC_DIR})
include_directories(${SRC_DIR}/../../src)
include_directories(${SRC_DIR}/../../src/emc/pythonplugin)
include_directories(${Python3_INCLUDE_DIRS})

# Set prefix.
set(CMAKE_SHARED_LIBRARY_PREFIX "")

ADD_LIBRARY(${PROJECT} SHARED
    ${SRC_DIR}/../../src/emc/pythonplugin/python_plugin.cc
    )

if (BUILD_USPACE)
    add_definitions(-DULAPI)
else ()
    add_definitions(-DRTAPI)
endif ()


# Set include paths for libraries to link.
add_library(linuxcncini SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(linuxcncini PROPERTIES
    IMPORTED_LOCATION "${SRC_DIR}/../build/liblinuxcncini/liblinuxcncini.so"
    INTERFACE_INCLUDE_DIRECTORIES "${SRC_DIR}/../../src/libnml/inifile"
)

# Link to libraries.
target_link_libraries(${PROJECT_NAME} PRIVATE linuxcncini -fPIC
    -I/usr/lib ${PYTHON_LIB}
    -lstdc++
    Boost::python${BOOST_PYTHON_LIB_SUFFIX} -lcrypt -lpthread -ldl -lutil -lm -lm
    -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions
)

# Add verion nr. .so.0.0.0
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# Set custom output name :
# set_property(TARGET  ${PROJECT} PROPERTY OUTPUT_NAME thename)

# Make install
install(TARGETS ${PROJECT} DESTINATION ${BUILD_DIR}/../../../lib)

# Notes.
