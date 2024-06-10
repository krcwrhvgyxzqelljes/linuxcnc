cmake_minimum_required(VERSION 3.5)
set(PROJECT "qt_lcnc_controller") # The name of the file excluding .c or .h
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
include_directories(${SRC_DIR}/../../vendor)
include_directories(${SRC_DIR}/../../vendor/occ_draw)
include_directories(${SRC_DIR}/../../vendor/oce-upstream-V7_5_0beta/inc)
include_directories(${SRC_DIR}/../../vendor/oce-upstream-V7_5_0beta/src)
include_directories(${SRC_DIR}/../../vendor/cavalier_contours/c_api_include)
include_directories(${SRC_DIR}/../../vendor/cavalier_contours/include/cavc)
include_directories(${SRC_DIR}/../../vendor/cavalier_contours/include)
include_directories(${SRC_DIR}/../../vendor/cavalier_contours/src)
include_directories(${SRC_DIR}/../../vendor/OpenNURBS/src/geom_defs)
include_directories(${SRC_DIR}/../../vendor/OpenNURBS/src/geom_defs)
include_directories(${SRC_DIR}/../../vendor/clothoid_andrew)
include_directories(${SRC_DIR}/../../vendor/scurvy_andrew)
include_directories(${SRC_DIR}/../../vendor/Clothoids/src)
include_directories(${SRC_DIR}/../../vendor/Clothoids/src/Clothoids)
include_directories(${SRC_DIR}/../../vendor/Clothoids/build)
include_directories(${SRC_DIR}/../../vendor/Clothoids/lib3rd/include)
include_directories(${SRC_DIR}/../../vendor/Clothoids/lib3rd/include/Utils)

include_directories("usr/include")
include_directories("usr/local/lib")
include_directories("usr/local/include")

include_directories(${Python3_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

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

add_library(clothoid SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(clothoid PROPERTIES
    IMPORTED_LOCATION ${SRC_DIR}/../../../lib/libclothoid.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../../include
)

add_library(scurvy SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(scurvy PROPERTIES
    IMPORTED_LOCATION ${SRC_DIR}/../../../lib/libscurvy.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../../include
)

add_library(clothoids SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(clothoids PROPERTIES
    IMPORTED_LOCATION ${SRC_DIR}/../../vendor/Clothoids/build/libClothoids_linux.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../vendor/Clothoids/lib/include
)

add_library(flocke SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(flocke PROPERTIES
    IMPORTED_LOCATION ${SRC_DIR}/../../vendor/Clothoids/lib3rd/dll/libquarticRootsFlocke_linux.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../vendor/Clothoids/lib3rd/include
)

add_library(utils SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(utils PROPERTIES
    IMPORTED_LOCATION ${SRC_DIR}/../../vendor/Clothoids/lib3rd/dll/libUtils_linux.so
    INTERFACE_INCLUDE_DIRECTORIES ${SRC_DIR}/../../vendor/Clothoids/lib3rd/include
)

# Opencascade
find_package (OpenCASCADE REQUIRED)

SET(OpenCASCADE_LIBS
    TKGeomAlgo TKMesh TKHLR TKBO TKShHealing
    TKPrim
    TKernel TKMath TKTopAlgo TKService
    TKG2d TKG3d TKV3d TKOpenGl
    TKBRep TKXSBase TKGeomBase TKGeomAlgo
    TKXSDRAW
    TKLCAF TKXCAF TKCAF TKVCAF
    TKCDF TKBin TKBinL TKBinXCAF TKXml TKXmlL TKXmlXCAF
    # -- IGES support
    TKIGES
    # -- STEP support
    TKSTEP TKXDESTEP TKXDEIGES TKSTEPAttr TKSTEPBase TKSTEP209
    # -- STL support
    TKSTL
    # -- OBJ/glTF support
    TKRWMesh TKMeshVS
    # -- VRML support
    TKVRML
    # -- ViewerTest
    TKViewerTest
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
    ${SRC_DIR}/form.cpp
    ${SRC_DIR}/form.h
    ${SRC_DIR}/form.ui
    ${SRC_DIR}/OcctQtViewer.h
    ${SRC_DIR}/OcctQtViewer.cpp
    ${SRC_DIR}/icons.qrc
    ${SRC_DIR}/../../vendor/occ_draw/draw_primitives.h
    ${SRC_DIR}/../../vendor/occ_draw/draw_primitives.cpp
    ${SRC_DIR}/../../vendor/occ_draw/draw_clothoids.h
    ${SRC_DIR}/../../vendor/occ_draw/draw_clothoids.cpp
    ${SRC_DIR}/interpreter.cpp
    ${SRC_DIR}/common.h
    ${SRC_DIR}/nml.h

    ${SRC_DIR}/QGCodeEditor.cpp
    ${SRC_DIR}/QGCodeSyntaxHighlighter.cpp
    ${SRC_DIR}/QGCodeEditor.h
    ${SRC_DIR}/QGCodeSyntaxHighlighter.h

    ${SRC_DIR}/../../../src/emc/sai/saicanon.cc
    ${SRC_DIR}/../../../src/emc/task/taskclass.cc

    ${SRC_DIR}/../../vendor/cavalier_contours/cavalier_offset.h
    ${SRC_DIR}/../../vendor/cavalier_contours/cavalier_offset.cpp
    ${SRC_DIR}/../../vendor/cavalier_contours/cavalier_3d.h
    ${SRC_DIR}/../../vendor/cavalier_contours/cavalier_3d.cpp
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
    # linuxcnchal linuxcncini linuxcnc_a nml rs274_
    ${OpenCASCADE_LIBS} Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARY}
    -I/usr/lib ${PYTHON_LIB} Boost::python${BOOST_PYTHON_LIB_SUFFIX} -lcrypt -ldl -lutil -lm -lreadline  -lpthread
    linuxcncini linuxcnc_a  rs274_ tooldata posemath pyplugin
    nml linuxcncini linuxcnchal -Wl,-rpath=../../../lib/
    clothoid scurvy clothoids
    flocke utils
)

# Make install
install(TARGETS ${PROJECT} DESTINATION ${BUILD_DIR}/../../../../bin)
