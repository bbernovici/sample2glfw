cmake_minimum_required(VERSION 3.13)
project(sample2glfw)

set(CMAKE_CXX_STANDARD 14)
set(SOURCES sample2.cpp Quad.h Point3d.h)

add_executable(sample2glfw ${SOURCES})
target_link_libraries(sample2glfw -lrfftw -lfftw -ldl -lglfw -lGL -lGLU -lGLEW -lm -lftgl -lglut)
