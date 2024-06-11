cmake_minimum_required(VERSION 3.5)
set(PROJECT "qt_sai") # The name of the file excluding .c or .h
project(${PROJECT} CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set variables.
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Set includepaths for header files etc.
include_directories(${SRC_DIR})
include_directories(${SRC_DIR}/../../../src)
include_directories(${SRC_DIR}/../../../src/emc/sai)
include_directories(${SRC_DIR}/../../../src/emc/task)
include_directories(${SRC_DIR}/../../../src/emc/pythonplugin)
include_directories(${SRC_DIR}/../../../src/hal)
include_directories(${SRC_DIR}/../../../src/rtapi)
include_directories(${SRC_DIR}/../../../src/emc)
include_directories(${SRC_DIR}/../../../src/emc/kinematics)
include_directories(${SRC_DIR}/../../../src/emc/motion-logger)
include_directories(${SRC_DIR}/../../../src/emc/tooldata)
include_directories(${SRC_DIR}/../../../src/emc/iotask)
include_directories(${SRC_DIR}/../../../src/emc/nml_intf)
include_directories(${SRC_DIR}/../../../src/emc/motion)
include_directories(${SRC_DIR}/../../../src/emc/rs274ngc)
include_directories(${SRC_DIR}/../../../src/emc/tp)
include_directories(${SRC_DIR}/../../../src/emc/ini)
include_directories(${SRC_DIR}/../../../src/libnml/rcs)
include_directories(${SRC_DIR}/../../../src/libnml/nml)
include_directories(${SRC_DIR}/../../../src/libnml/cms)
include_directories(${SRC_DIR}/../../../src/libnml/buffer)
include_directories(${SRC_DIR}/../../../src/libnml/posemath)
include_directories(${SRC_DIR}/../../../src/libnml/inifile)
include_directories(${SRC_DIR}/../../../src/libnml/os_intf)

include_directories("usr/include")
include_directories("usr/local/lib")
include_directories("usr/local/include")

include_directories(${Python3_INCLUDE_DIRS})

# Set path to eigen3, for install $ sudo apt-get install libeigen3-dev
include_directories("/usr/include/eigen3")

# Set include paths for libraries to link.
add_library(linuxcnchal SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(linuxcnchal PROPERTIES
    IMPORTED_LOCATION ${SRC_DIR}/../../../lib/liblinuxcnchal.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../../include
)

add_library(linuxcncini SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(linuxcncini PROPERTIES
    IMPORTED_LOCATION ${SRC_DIR}/../../../lib/liblinuxcncini.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../../include
)

add_library(linuxcnc_a STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(linuxcnc_a PROPERTIES
    # IMPORTED_LOCATION "/home/user/lcncladdertest/lib/liblinuxcnc.a"
    IMPORTED_LOCATION ${SRC_DIR}/../../../lib/liblinuxcnc.a
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../../include
)

# Set include paths for libraries to link.
add_library(nml SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(nml PROPERTIES
    IMPORTED_LOCATION ${SRC_DIR}/../../../lib/libnml.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../../include
)

add_library(rs274_ SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(rs274_ PROPERTIES
    IMPORTED_LOCATION ${SRC_DIR}/../../../lib/librs274.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../../include
)

add_library(tooldata SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(tooldata PROPERTIES
    # IMPORTED_LOCATION "/home/user/lcncladdertest/lib/libtooldata.so"
    IMPORTED_LOCATION ${SRC_DIR}/../../../lib/libtooldata.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../../include
)

add_library(pyplugin SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(pyplugin PROPERTIES
    IMPORTED_LOCATION ${SRC_DIR}/../../../lib/libpyplugin.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../../include # Todo: figur out the excact includes for this one.
)

add_library(posemath SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(posemath PROPERTIES
    IMPORTED_LOCATION ${SRC_DIR}/../../../lib/libposemath.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../../include
)

# Qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "/opt/qt-creator/5.15.1/gcc_64/lib/cmake")
include_directories("/opt/qt-creator/5.15.1/gcc_64/include/QtWidgets")
include_directories("/opt/qt-creator/5.15.1/gcc_64/include/QtGui")
include_directories("/opt/qt-creator/5.15.1/gcc_64/include")
include_directories("/opt/qt-creator/5.15.1/gcc_64/include/QtCore")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets OpenGL REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets OpenGL REQUIRED)

set(PROJECT_SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/mainwindow.cpp
    ${SRC_DIR}/mainwindow.h
    ${SRC_DIR}/mainwindow.ui
    ${SRC_DIR}/interpreter_intf.h
    ${SRC_DIR}/interpreter_intf.cpp
    ${SRC_DIR}/../../../src/emc/sai/saicanon.hh
    ${SRC_DIR}/../../../src/emc/sai/saicanon.cc
    ${SRC_DIR}/../../../src/emc/task/taskclass.cc
    # ${SRC_DIR}/../../../src/emc/nml_intf/canon.hh
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT}
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(${PROJECT} SHARED
            ${PROJECT_SOURCES}
        )
else()
    add_executable(${PROJECT}
        ${PROJECT_SOURCES}
    )
endif()
endif()

if (BUILD_USPACE)
    add_definitions(-DULAPI)
else ()
    add_definitions(-DRTAPI)
endif ()

# Link.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Widgets Qt5::OpenGL
    -I/usr/lib ${PYTHON_LIB} Boost::python${BOOST_PYTHON_LIB_SUFFIX} -lcrypt -ldl -lutil -lm -lreadline  -lpthread
    linuxcncini linuxcnc_a  rs274_ tooldata posemath pyplugin
    nml linuxcncini linuxcnchal -Wl,-rpath=../../../lib/
)

# Make install
install(TARGETS ${PROJECT} DESTINATION ${BUILD_DIR}/../../../../bin)
