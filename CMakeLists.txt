cmake_minimum_required(VERSION 3.21)

if (WIN32)
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
endif()

project(UVKBuildToolLib)
project(UVKBuildTool)

# This option is needed to define a compiler macro and the given source files to be used by the compilation target
option(UBT_COMPILING_FOR_WEB "If set to ON it will compile with configuration for a web project, otherwise will compile for the UntitledImGuiFramework!" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (WIN32)
    set(BUILD_SHARED_LIBS ON)
    add_subdirectory(yaml-cpp)
    set(BUILD_SHARED_LIBS OFF)
else()
    include(FindYamlCpp)
    find_package(YamlCpp REQUIRED)
    include_directories(${YamlCpp_INCLUDE_PATH})
    cmake_path(GET YamlCpp_LIBRARY PARENT_PATH YAML_PARENT_DIR)
    link_directories(${YAML_PARENT_DIR})
endif()

if (WIN32)
    add_subdirectory(ucli/)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(UntitledCLIParser UntitledCLIParser)
    if (UntitledCLIParser_FOUND)
        link_directories(${UntitledCLIParser_LIBRARY_DIRS})
        include_directories(${UntitledCLIParser_INCLUDE_DIRS})
    else()
        add_subdirectory(ucli/)
    endif()
endif()


if (UBT_COMPILING_FOR_WEB)
    set(BUILD_VARIANT_VENDOR ON)
    set(USE_PRECONFIGURED_YAML ON)
    add_subdirectory(ui18n)
endif()

set(YAML_CPP_INCLUDE_DIRS_T "yaml-cpp/include" "yaml-cpp/include/yaml-cpp")
include_directories(${YAML_CPP_INCLUDE_DIRS_T} utg/src/ src/ ui18n/)

if (UBT_COMPILING_FOR_WEB)
    file(GLOB_RECURSE SRC "src/Web/*.cpp" )
    file(GLOB_RECURSE HEAD "src/Web/*.hpp" )
else()
    file(GLOB_RECURSE SRC "src/UntitledImGuiFramework/*.cpp")
    file(GLOB_RECURSE HEAD "src/UntitledImGuiFramework/*.hpp")
endif()
file(GLOB_RECURSE UNI_HEAD "src/Uniform/*.h" "utg/src/*.hpp" "utg/src/*.h" "ui18n/parallel-hashmap/*.h")
file(GLOB_RECURSE UNI_SRC "src/Uniform/*.cpp")
file(GLOB_RECURSE UTG_SRC "utg/src/*.cpp")

add_library(UVKBuildToolLib SHARED ${SRC} ${HEAD} ${UNI_SRC} ${UNI_HEAD} ${UTG_SRC})
add_executable(UVKBuildTool ${HEAD} ${UNI_HEAD} main.cpp)

set_target_properties(UVKBuildToolLib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(UVKBuildTool PROPERTIES LINKER_LANGUAGE CXX)

target_compile_definitions(UVKBuildToolLib PRIVATE UTTE_CUSTOM_MAP=phmap::flat_hash_map UTTE_CUSTOM_MAP_INCLUDE=<parallel-hashmap/parallel_hashmap/phmap.h>)
target_compile_definitions(UVKBuildTool PRIVATE UTTE_CUSTOM_MAP=phmap::flat_hash_map UTTE_CUSTOM_MAP_INCLUDE=<parallel-hashmap/parallel_hashmap/phmap.h>)
if (UBT_COMPILING_FOR_WEB)
    target_compile_definitions(UVKBuildToolLib PRIVATE "UBT_LIB_COMPILE" "MLS_LIB_COMPILE" "YAML_CPP_DLL" "UBT_TARGET_WEB" "MLS_EXPORT_LIBRARY")
    target_compile_definitions(UVKBuildTool PRIVATE "UBT_TARGET_WEB" "MLS_EXPORT_LIBRARY")
else()
    target_compile_definitions(UVKBuildToolLib PRIVATE "UBT_LIB_COMPILE" "MLS_LIB_COMPILE" "YAML_CPP_DLL" "UBT_TARGET_FRAMEWORK" "MLS_EXPORT_LIBRARY")
    target_compile_definitions(UVKBuildTool PRIVATE "UBT_TARGET_FRAMEWORK" "MLS_EXPORT_LIBRARY")
endif()

target_link_libraries(UVKBuildToolLib yaml-cpp)
if (UBT_COMPILING_FOR_WEB)
    target_link_libraries(UVKBuildToolLib UntitledI18N)
endif()

target_link_libraries(UVKBuildTool UVKBuildToolLib UntitledCLIParser)
