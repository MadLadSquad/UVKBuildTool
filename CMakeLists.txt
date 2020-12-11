cmake_minimum_required(VERSION 3.17)
project(UVKBuildTool)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(yaml-cpp)

include_directories(yaml-cpp/include)
include_directories(yaml-cpp/include/yaml-cpp)

add_executable(UVKBuildTool src/main.cpp src/Utility.h)
target_link_libraries(UVKBuildTool yaml-cpp)