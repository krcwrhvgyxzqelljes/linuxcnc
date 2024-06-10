#-----------------------------------------------------------------------#
#  file: CMakeLists.txt                                                 #
#                                                                       #
#  version: 1.0   date 9/3/2022                                         #
#                                                                       #
#  Copyright (C) 2022                                                   #
#                                                                       #
#      Enrico Bertolazzi, Francesco Biral and Paolo Bosetti             #
#      Dipartimento di Ingegneria Industriale                           #
#      Universita` degli Studi di Trento                                #
#      Via Sommarive 9, I-38123, Trento, Italy                          #
#      email: enrico.bertolazzi@unitn.it                                #
#             francesco.biral@unitn.it                                  #
#             paolo.bosetti@unitn.it                                    #
#-----------------------------------------------------------------------#

cmake_minimum_required( VERSION 3.14 )

project( AABB )

include(../cmake_utils/CMakeLists-common.txt)

project( ${PROJECT_NAME} VERSION ${UTILS_PROJECT_VERSION} )

find_package( Matlab REQUIRED )

message( STATUS "Matlab_ROOT_DIR = ${Matlab_ROOT_DIR}" )
message( STATUS "PROJECT_NAME    = ${PROJECT_NAME}" )

if ( (UNIX OR LINUX) AND NOT APPLE )
  set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
  set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

include(../cmake_utils/CMakeLists-cflags.txt)
include(../cmake_utils/CMakeLists-utilities.txt)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Evaluating the suffix to append to target built by this CMake file
utils_artifacts_suffix(ARTIFACTS_STATIC_SUFFIX TRUE)
if( UTILS_BUILD_SHARED )
  utils_artifacts_suffix(ARTIFACTS_DYNAMIC_SUFFIX FALSE)
endif()

message( STATUS "Compiler used: ${CMAKE_CXX_COMPILER_ID}" )
message( STATUS "BASE SUFFIXES = ${ARTIFACTS_STATIC_SUFFIX} ${ARTIFACTS_DYNAMIC_SUFFIX}" )

#   ____                                _     _ _
#  |  _ \ _ __ ___ _ __ ___  __ _ _   _(_)___(_) |_ ___
#  | |_) | '__/ _ \ '__/ _ \/ _` | | | | / __| | __/ _ \
#  |  __/| | |  __/ | |  __/ (_| | |_| | \__ \ | ||  __/
#  |_|   |_|  \___|_|  \___|\__, |\__,_|_|___/_|\__\___|
#                              |_|
# MUST be executed before the build of the header list

#   ___
#  / __| ___ _  _ _ _ __ ___ ___
#  \__ \/ _ \ || | '_/ _/ -_|_-<
#  |___/\___/\_,_|_| \__\___/__/
#

set( SOURCES )
file(
  GLOB_RECURSE S
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cc
)
foreach(F ${S})
  # build relative path respect to source directory
  file( RELATIVE_PATH RF ${CMAKE_CURRENT_SOURCE_DIR} "${F}" )
  list( APPEND SOURCES ${RF} )
endforeach(F ${S})

#   _  _             _
#  | || |___ __ _ __| |___ _ _ ___
#  | __ / -_) _` / _` / -_) '_(_-<
#  |_||_\___\__,_\__,_\___|_| /__/
#
set( PUBLIC_HEADERS )
file(
  GLOB_RECURSE S
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.h*
)
foreach (F ${S})
  # build relative path respect to source directory
  file( RELATIVE_PATH RF "${CMAKE_CURRENT_SOURCE_DIR}" "${F}" )
  list( APPEND PUBLIC_HEADERS ${RF} )
endforeach(F ${S})

list(FILTER PUBLIC_HEADERS EXCLUDE REGEX "\.DS")

# extra include directory for compilation
include_directories( src ../src ../src/Utils )

message( STATUS "SOURCES = ${SOURCES}" )

#   _____                  _
#  |_   _|_ _ _ _ __ _ ___| |_ ___
#    | |/ _` | '_/ _` / -_)  _(_-<
#    |_|\__,_|_| \__, \___|\__/__/
#                |___/
#
# define target
utils_setup_target(
  ${PROJECT_NAME}
  TRUE # build static
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${SOURCES}"
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${PUBLIC_HEADERS}"
)
if ( UTILS_BUILD_SHARED )
  utils_setup_target(
    ${PROJECT_NAME}
    FALSE # build dynamic
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${SOURCES}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${PUBLIC_HEADERS}"
  )
endif()

#   _____       _
#  |_   _|__ __| |_ ___
#    | |/ -_|_-<  _(_-<
#    |_|\___/__/\__/__/
#
if ( UTILS_ENABLE_TESTS )

  enable_testing()

  set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin )
  set( LIBS ${UTILS_NAMESPACE}_${PROJECT_NAME}_Static ${CMAKE_DL_LIBS} )

  if( UNIX AND NOT APPLE )
    set( THREADS_PREFER_PTHREAD_FLAG ON )
    find_package( Threads REQUIRED )
    set( LIBS ${LIBS} Threads::Threads )
  endif()

  set( EXELISTCPP )
  file(
    GLOB S
    ${CMAKE_CURRENT_SOURCE_DIR}/src_tests/*.cc
  )
  foreach(F ${S})
    file( RELATIVE_PATH RF "${CMAKE_CURRENT_SOURCE_DIR}/src_tests/" "${F}" )
    get_filename_component( RFF ${RF} NAME_WLE )
    list( APPEND EXELISTCPP ${RFF} )
  endforeach(F ${S})

  add_custom_target( "${PROJECT_NAME}_all_tests" ALL )

  foreach( S ${EXELISTCPP} )
    add_executable( ${S} ${CMAKE_CURRENT_SOURCE_DIR}/src_tests/${S}.cc )
    target_link_libraries( ${S} ${LIBS} )
    add_test( NAME "${S}" COMMAND ./bin/${S} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
    add_dependencies( "${PROJECT_NAME}_all_tests" ${S} )
  endforeach()

endif()

#   ___         _        _ _
#  |_ _|_ _  __| |_ __ _| | |
#   | || ' \(_-<  _/ _` | | |
#  |___|_||_/__/\__\__,_|_|_|
#
install(
  TARGETS ${TARGETS_TO_BE_INSTALLED}
  # EXPORT (da definire)
  LIBRARY DESTINATION ${INSTALL_PREFIX}/dll
  ARCHIVE DESTINATION ${INSTALL_PREFIX}/lib
  RUNTIME DESTINATION ${INSTALL_PREFIX}/bin
  PUBLIC_HEADER DESTINATION ${PROJECT_PUBLIC_HEADER_DESTINATION}
)

utils_final_messages()

add_library( Utils_static STATIC ${SOURCES} )

set( MEX_LIST AABB_tree Segment2D )
foreach(MEX ${MEX_LIST})
  matlab_add_mex(
    NAME mex_${MEX}
    SRC src_mex/mex_${MEX}.cc
    LINK_TO Utils_static
    OUTPUT_NAME ${MEX}MexWrapper
  )
  add_custom_command(
    TARGET mex_${MEX}
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
    "$<TARGET_FILE:mex_${MEX}>"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"
  )
endforeach()
