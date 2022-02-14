cmake_minimum_required(VERSION 3.17)
project(UVKBuildTool)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(yaml-cpp)

include_directories(yaml-cpp/include)
include_directories(yaml-cpp/include/yaml-cpp)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.hpp")

add_executable(UVKBuildTool ${SRC})
target_link_libraries(UVKBuildTool yaml-cpp)
