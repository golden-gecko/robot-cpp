cmake_minimum_required(VERSION 3.0)
project(Robot)

# Include files
include_directories("D:/Dev/boost_1_55_0")
include_directories("D:/Dev/ois-v1-3/includes")

# Libraries
link_directories("D:/Dev/ois-v1-3/lib")
link_directories("D:/Dev/boost_1_55_0/stage/lib")

# Source files
add_executable(Robot Input.hpp Main.cpp Window.hpp)

# Libraries
target_link_libraries(Robot OIS_static)

# OpenGL
find_package(OpenGL REQUIRED)

if(NOT OPENGL_FOUND)
  message(ERROR "OpenGL not found")
endif(NOT OPENGL_FOUND)

include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
target_link_libraries(Robot ${OPENGL_LIBRARIES})
