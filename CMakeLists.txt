cmake_minimum_required(VERSION 3.17)
project(UVKBuildToolLib)
project(UVKBuildTool)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS ON)
add_subdirectory(yaml-cpp)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(utg/ThirdParty/utfcpp)

include_directories(yaml-cpp/include)
include_directories(yaml-cpp/include/yaml-cpp)
include_directories(utg/src/)
include_directories(utg/ThirdParty/utfcpp/source/)

file(GLOB_RECURSE SRC "src/*.cpp" "utg/src/*.cpp")
file(GLOB_RECURSE HEAD "src/*.hpp" "utg/src/*.hpp")

add_library(UVKBuildToolLib SHARED ${SRC} ${HEAD})
add_executable(UVKBuildTool ${HEAD} main.cpp)

set_target_properties(UVKBuildTool PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(UVKBuildToolLib PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(UVKBuildToolLib PRIVATE "UBT_LIB_COMPILE" "YAML_CPP_DLL")

target_link_libraries(UVKBuildToolLib yaml-cpp)
target_link_libraries(UVKBuildTool UVKBuildToolLib yaml-cpp)
