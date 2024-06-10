cmake_minimum_required(VERSION 3.5)

project(angular_motion LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
    main.cpp
    angular_motion.h
    angular_motion.cpp
)

add_executable(runtest ${PROJECT_SOURCES} )


