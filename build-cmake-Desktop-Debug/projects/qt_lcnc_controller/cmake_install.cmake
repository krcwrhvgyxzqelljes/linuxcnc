# Install script for directory: /home/user/linuxcnc/cmake/projects/qt_lcnc_controller

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump-14")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/user/linuxcnc/build-cmake-Desktop-Debug/projects/qt_lcnc_controller/../../../../bin/qt_lcnc_controller" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/user/linuxcnc/build-cmake-Desktop-Debug/projects/qt_lcnc_controller/../../../../bin/qt_lcnc_controller")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/user/linuxcnc/build-cmake-Desktop-Debug/projects/qt_lcnc_controller/../../../../bin/qt_lcnc_controller"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/user/linuxcnc/build-cmake-Desktop-Debug/projects/qt_lcnc_controller/../../../../bin/qt_lcnc_controller")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/user/linuxcnc/build-cmake-Desktop-Debug/projects/qt_lcnc_controller/../../../../bin" TYPE EXECUTABLE FILES "/home/user/linuxcnc/build-cmake-Desktop-Debug/projects/qt_lcnc_controller/qt_lcnc_controller")
  if(EXISTS "$ENV{DESTDIR}/home/user/linuxcnc/build-cmake-Desktop-Debug/projects/qt_lcnc_controller/../../../../bin/qt_lcnc_controller" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/user/linuxcnc/build-cmake-Desktop-Debug/projects/qt_lcnc_controller/../../../../bin/qt_lcnc_controller")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/user/linuxcnc/build-cmake-Desktop-Debug/projects/qt_lcnc_controller/../../../../bin/qt_lcnc_controller"
         OLD_RPATH "/usr/local/lib:/opt/qt-creator/5.15.1/gcc_64/lib:/home/user/linuxcnc/cmake/projects/qt_lcnc_controller/../../vendor/Clothoids/build:/home/user/linuxcnc/cmake/projects/qt_lcnc_controller/../../vendor/Clothoids/lib3rd/dll:/home/user/linuxcnc/cmake/projects/qt_lcnc_controller/../../../lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-14" "$ENV{DESTDIR}/home/user/linuxcnc/build-cmake-Desktop-Debug/projects/qt_lcnc_controller/../../../../bin/qt_lcnc_controller")
    endif()
  endif()
endif()

