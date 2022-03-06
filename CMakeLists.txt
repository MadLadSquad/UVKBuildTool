cmake_minimum_required(VERSION 3.17)
project(UVKBuildTool)
project(UVKBuildToolLib)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(yaml-cpp)

include_directories(yaml-cpp/include)
include_directories(yaml-cpp/include/yaml-cpp)

file(GLOB_RECURSE SRC "src/*.cpp")
file(GLOB_RECURSE HEAD "src/*.hpp")

set_target_properties(UVKBuildTool PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(UVKBuildToolLib PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(UVKBuildToolLib PRIVATE "UBT_LIB_COMPILE")
add_library(UVKBuildToolLib SHARED ${SRC} ${HEAD})
target_link_libraries(UVKBuildToolLib yaml-cpp)

add_executable(UVKBuildTool ${HEAD} main.cpp)
target_link_libraries(UVKBuildTool UVKBuildToolLib)