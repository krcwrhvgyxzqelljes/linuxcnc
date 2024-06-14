## Tip: usefull debug when compiling source in it's original way : make BUILD_VERBOSE=1
# Tip: For reverse engineering, by just deleting one .so lib, then invoke "make BUILD_VERBOSE=1"
# It shows how its build and linked.

# Note :
# Components are linked to liblinuxcnchal in ~/linuxcnc/lib. If you build liblinuxcnchal with cmake, you have to
# build the project in 2 steps. First build & install liblinuxcnchal, second build the components afterwards.

# Note :
# After "make install", you could consider "~/src/sudo make setuid".

# Problems :
# Duplicate library names, cmake policy. To set output name :
# ~/linuxcnc/tcl/linuxcnc.so is duplicate.
# set_property(TARGET  ${PROJECT} PROPERTY OUTPUT_NAME linuxcnc)

# Tip :
# If you have problems with solving compile errors. What worked for me is when problems occur, include the original lcnc compiled libs first
# in your project, unitl the build finishes ok. Then switch out libs step by step with your own compiled libs. Use full filepath's to be sure.

# Important :
#
# After clean build and install :   ~/src : sudo make setuid
#

# Todo :
# Integrate halcompile .comp to .so process.

cmake_minimum_required(VERSION 3.0)
# Project name.
set(PROJECT "- LINUX_CNC -") # The name of the file excluding .c or .h
project(${PROJECT} CXX C) # Asm is optional.

set(CMAKE_CXX_STANDARD 17)

# Set variables.
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Build and install the core first.
# ~/bin (c++ kernel core code)
add_subdirectory(${SRC_DIR}/rtapi)
add_subdirectory(${SRC_DIR}/linuxcnc_module_helper)

# If RPC.H errors
include_directories("/usr/include/tirpc/")

# Lets sort out the Python / Boost Python versioning
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
string(REPLACE "." "" PYTHON_SUFFIX ${Python3_VERSION})
string(REGEX MATCH "^([0-9]+)\\.([0-9]+)" _ ${Python3_VERSION})
set(PYTHON_MAJOR_VERSION ${CMAKE_MATCH_1})
set(PYTHON_MINOR_VERSION ${CMAKE_MATCH_2})
set(PYTHON_LIB "python${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}")
set(BOOST_PYTHON_LIB_SUFFIX "${PYTHON_MAJOR_VERSION}${PYTHON_MINOR_VERSION}")
find_package(Boost REQUIRED COMPONENTS python${BOOST_PYTHON_LIB_SUFFIX})
#[[
So we end up with the following
Script header       #!/usr/bin/env python3
Includes            include_directories(${Python3_INCLUDE_DIRS})
Python Library      ${PYTHON_LIB}
Boost library       Boost::python${BOOST_PYTHON_LIB_SUFFIX}
]]

# add_subdirectory(${SRC_DIR}/vendor/ruckig_dev)

include_directories("/usr/include/boost")

# ~/lib (c - c++ code for motion, gcode etc.)
add_subdirectory(${SRC_DIR}/liblinuxcnchal)
add_subdirectory(${SRC_DIR}/liblinuxcncini)
add_subdirectory(${SRC_DIR}/libtooldata)
add_subdirectory(${SRC_DIR}/libnml)
add_subdirectory(${SRC_DIR}/liblinuxcnc)
add_subdirectory(${SRC_DIR}/libposemath)
add_subdirectory(${SRC_DIR}/libpyplugin)
add_subdirectory(${SRC_DIR}/librs274)
add_subdirectory(${SRC_DIR}/libcanterp)

## ~/rtlib (realtime modules for real hardware or simulating)
#add_subdirectory(${SRC_DIR}/hal_lib)
#add_subdirectory(${SRC_DIR}/components/test)
#add_subdirectory(${SRC_DIR}/components/tp)
add_subdirectory(${SRC_DIR}/components/tpmod) # Rob Ellenberg's motion planner.
##add_subdirectory(${SRC_DIR}/components/tpmod_runners) # Rob Ellenberg's motion planner testing some scurve runners.
##add_subdirectory(${SRC_DIR}/components/tpmod_template) # a template how the lcnc program flow works.


 # working scurve jog planner. Set's the maxjerk to be 2*maxacc.
#add_subdirectory(${SRC_DIR}/components/tpmod_scurve)

#add_subdirectory(${SRC_DIR}/components/tpmod_scurve_skynet) # minimal coded grotius scurve planner, run's program forward and pause.

# In development, based on the tpmod_scurve_skynet.
# Known bug : jogging before program start, solves a position jump when pressing program stop.
 add_subdirectory(${SRC_DIR}/components/tpmod_scurve_grotius)
# add_subdirectory(${SRC_DIR}/components/tpmod_scurve_c++)

##add_subdirectory(${SRC_DIR}/components/tpmod_T800) # experimental scurve lib using ruckig-dev, enabling velocity ends.
#add_subdirectory(${SRC_DIR}/components/homemod)
#add_subdirectory(${SRC_DIR}/components/threads)
#add_subdirectory(${SRC_DIR}/components/ethercat)
#add_subdirectory(${SRC_DIR}/components/lineardeltakins)
#add_subdirectory(${SRC_DIR}/components/rotarydeltakins)
#add_subdirectory(${SRC_DIR}/components/pumakins)
#add_subdirectory(${SRC_DIR}/components/genserkins) # robot
#add_subdirectory(${SRC_DIR}/components/5axiskins) # bridge mill 5 axis
#add_subdirectory(${SRC_DIR}/components/trivkins)
#add_subdirectory(${SRC_DIR}/components/rotatekins)
#add_subdirectory(${SRC_DIR}/components/xyzac-trt-kins)
#add_subdirectory(${SRC_DIR}/components/xyzbc-trt-kins)
#add_subdirectory(${SRC_DIR}/components/switchkins)
#add_subdirectory(${SRC_DIR}/components/pentakins)
#add_subdirectory(${SRC_DIR}/components/scorbotkins)
#add_subdirectory(${SRC_DIR}/components/tripodkins)
#add_subdirectory(${SRC_DIR}/components/scarakins)
#add_subdirectory(${SRC_DIR}/components/maxkins)
#add_subdirectory(${SRC_DIR}/components/corexykins)
#add_subdirectory(${SRC_DIR}/components/genhexkins)
#add_subdirectory(${SRC_DIR}/components/rosekins)
#add_subdirectory(${SRC_DIR}/components/ugenserkins)
#add_subdirectory(${SRC_DIR}/components/scurve)
add_subdirectory(${SRC_DIR}/components/motmod) # lcnc motion controller component. also has jogging trapezium
#add_subdirectory(${SRC_DIR}/components/motmod_scurve) # jogging grotius scurve
##add_subdirectory(${SRC_DIR}/components/motmod_T800) # jogging scurve using ruckig-dev lib.
#add_subdirectory(${SRC_DIR}/components/scope_rt)
#add_subdirectory(${SRC_DIR}/components/pid)
##add_subdirectory(${SRC_DIR}/components/cyberdyne)
##add_subdirectory(${SRC_DIR}/components/simple_tp)


## Components from here need a halcompile from .comp to .c file.
##add_subdirectory(${SRC_DIR}/components/ddt)
##add_subdirectory(${SRC_DIR}/components/hypot)
##add_subdirectory(${SRC_DIR}/components/sim_spindle)
##add_subdirectory(${SRC_DIR}/components/limit2)
##add_subdirectory(${SRC_DIR}/components/lowpass)
##add_subdirectory(${SRC_DIR}/components/near)
##add_subdirectory(${SRC_DIR}/components/scale)
##add_subdirectory(${SRC_DIR}/components/or2)
##add_subdirectory(${SRC_DIR}/components/comp)
##add_subdirectory(${SRC_DIR}/components/and2)

## ~/bin (executable commandline programs)
add_subdirectory(${SRC_DIR}/inivar)
add_subdirectory(${SRC_DIR}/halcmd) # atomic include error.
add_subdirectory(${SRC_DIR}/halmeter)
add_subdirectory(${SRC_DIR}/halscope)
add_subdirectory(${SRC_DIR}/classicladder)
add_subdirectory(${SRC_DIR}/halui)
add_subdirectory(${SRC_DIR}/halrmt)
add_subdirectory(${SRC_DIR}/pci_write)
add_subdirectory(${SRC_DIR}/pci_read)
#add_subdirectory(${SRC_DIR}/io)
#add_subdirectory(${SRC_DIR}/iov2)
add_subdirectory(${SRC_DIR}/motion_logger)
add_subdirectory(${SRC_DIR}/sai)
add_subdirectory(${SRC_DIR}/linuxcncsvr)
add_subdirectory(${SRC_DIR}/milltask)
add_subdirectory(${SRC_DIR}/linuxcncrsh)
add_subdirectory(${SRC_DIR}/schedrmt)
add_subdirectory(${SRC_DIR}/linuxcnclcd)

## ~/tlc (used before gtk was invented)
#add_subdirectory(${SRC_DIR}/linuxcnc_tcl)
#add_subdirectory(${SRC_DIR}/hal_tcl)

## ~/lib/python (c++ python glue code)
add_subdirectory(${SRC_DIR}/lineardeltakins_py)
add_subdirectory(${SRC_DIR}/rotarydeltakins_py)
add_subdirectory(${SRC_DIR}/hal_py)
add_subdirectory(${SRC_DIR}/togl_py)
add_subdirectory(${SRC_DIR}/linuxcnc_py)
add_subdirectory(${SRC_DIR}/gcode_py)

# ~/projects (c - c++ gui projects)
# add_subdirectory(${SRC_DIR}/projects/qt_nml) # Requires qt installed, first start linuxcnc, then start qt_nml.

# Requires qt & opencascade installed, opencascade version: ~/cmake/vendor/oce-upstream-V7_5_0beta
# add_subdirectory(${SRC_DIR}/projects/qt_gremlin)
# add_subdirectory(${SRC_DIR}/projects/qt_lcnc_controller)
add_subdirectory(${SRC_DIR}/projects/qt_sai)
# add_subdirectory(${SRC_DIR}/projects/qt_occ_draw)
# add_subdirectory(${SRC_DIR}/projects/qt_occ_cavalier)

#add_subdirectory(${SRC_DIR}/vendor/clothoid_andrew)

# add_subdirectory(${SRC_DIR}/projects/qt_occ_biclothoid_fillet)

# Experimental scurve motion planner.
# add_subdirectory(${SRC_DIR}/components/sc_tp)

#add_subdirectory(${SRC_DIR}/vendor/scurve)














