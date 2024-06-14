# Install script for directory: /home/user/linuxcnc/cmake

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/user/linuxcnc/cmake/build/rtapi/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/linuxcnc_module_helper/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/liblinuxcnchal/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/liblinuxcncini/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/libtooldata/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/libnml/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/liblinuxcnc/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/libposemath/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/libpyplugin/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/librs274/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/libcanterp/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/components/tpmod/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/components/motmod/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/inivar/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/halcmd/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/halmeter/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/halscope/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/classicladder/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/halui/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/halrmt/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/pci_write/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/pci_read/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/motion_logger/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/sai/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/linuxcncsvr/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/milltask/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/linuxcncrsh/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/schedrmt/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/linuxcnclcd/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/lineardeltakins_py/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/rotarydeltakins_py/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/hal_py/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/togl_py/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/linuxcnc_py/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/gcode_py/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/projects/qt_lcnc_controller/cmake_install.cmake")
  include("/home/user/linuxcnc/cmake/build/projects/qt_sai/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/user/linuxcnc/cmake/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
