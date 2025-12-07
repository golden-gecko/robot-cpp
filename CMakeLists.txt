cmake_minimum_required(VERSION 4.2)

project(robot)

# Include files
# include_directories("D:/Dev/boost_1_55_0")
# include_directories("D:/Dev/ois-v1-3/includes")

# Libraries
# link_directories("D:/Dev/ois-v1-3/lib")
# link_directories("D:/Dev/boost_1_55_0/stage/lib")

# Source files
add_executable(robot Input.hpp Main.cpp Window.hpp)

# Libraries
target_link_libraries(robot OIS_static)

# OpenGL
find_package(OpenGL REQUIRED)

if(NOT OPENGL_FOUND)
  message(ERROR "OpenGL not found")
endif(NOT OPENGL_FOUND)

include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
target_link_libraries(robot ${OPENGL_LIBRARIES})

set_target_properties(robot PROPERTIES
    CXX_EXTENSIONS NO
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
