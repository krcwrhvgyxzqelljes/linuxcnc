cmake_minimum_required(VERSION 3.5)
set(PROJECT "qt_occ_cavalier") # The name of the file excluding .c or .h
project(${PROJECT} C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set variables.
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Set includepaths for header files etc.
include_directories(${SRC_DIR})
include_directories(${SRC_DIR}/../../../src)
include_directories(${SRC_DIR}/../../vendor)
include_directories(${SRC_DIR}/../../vendor/oce-upstream-V7_5_0beta/inc)
include_directories(${SRC_DIR}/../../vendor/oce-upstream-V7_5_0beta/src)
include_directories(${SRC_DIR}/../../vendor/cavalier_contours/c_api_include)
include_directories(${SRC_DIR}/../../vendor/cavalier_contours/include/cavc)
include_directories(${SRC_DIR}/../../vendor/cavalier_contours/include)
include_directories(${SRC_DIR}/../../vendor/cavalier_contours/src)
include_directories(${SRC_DIR}/../../vendor/occ_draw)
include_directories("usr/include")
include_directories("usr/local/lib")
include_directories(${Python3_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories("/usr/include/eigen3")
find_package (Eigen3 3.3 REQUIRED)

# Opencascade, in the opencascade build dir do : make install
# If that don't work, see the howto.txt file in the occ folder.
# find_package (OpenCASCADE REQUIRED)

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
    ${SRC_DIR}/../../vendor/cavalier_contours/include/cavc/polylineoffset.hpp
    ${SRC_DIR}/../../vendor/cavalier_contours/src/cavaliercontours.cpp
    #${SRC_DIR}/../../vendor/cavalier_contours/cavalier_functions.h
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

# Link.
target_link_libraries(${PROJECT_NAME} PRIVATE
   -I/usr/local/lib  ${OpenCASCADE_LIBS} Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARY}
   -I/usr/lib ${PYTHON_LIB} Boost::python${BOOST_PYTHON_LIB_SUFFIX} -lm
)

# Make install
install(TARGETS ${PROJECT} DESTINATION ${BUILD_DIR}/../../../../bin)
