cmake_minimum_required(VERSION 3.17)
project(UVKBuildTool)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(yaml-cpp)


add_executable(UVKBuildTool main.cpp)