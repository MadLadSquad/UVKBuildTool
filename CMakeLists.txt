cmake_minimum_required(VERSION 3.21)

project(UVKBuildToolLib)
project(UVKBuildTool)

# This option is needed to define a compiler macro and the given source files to be used by the compilation target
option(UBT_COMPILING_FOR_WEB "If set to ON it will compile with configuration for a web project, otherwise will compile for the UntitledImGuiFramework!" OFF)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS ON)
add_subdirectory(yaml-cpp)
set(BUILD_SHARED_LIBS OFF)

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
file(GLOB_RECURSE UNI_HEAD "src/Uniform/*.h" "utg/src/*.hpp" "utg/src/*.h")
file(GLOB_RECURSE UNI_SRC "src/Uniform/*.cpp")
file(GLOB_RECURSE UTG_SRC "utg/src/*.cpp")

add_library(UVKBuildToolLib SHARED ${SRC} ${HEAD} ${UNI_SRC} ${UNI_HEAD} ${UTG_SRC})
add_executable(UVKBuildTool ${HEAD} ${UNI_HEAD} main.cpp)

set_target_properties(UVKBuildToolLib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(UVKBuildTool PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(UVKBuildTool PRIVATE UTTE_CUSTOM_MAP=phmap::flat_hash_map UTTE_CUSTOM_MAP_INCLUDE=<Uniform/parallel-hashmap/parallel_hashmap/phmap.h>)
if (UBT_COMPILING_FOR_WEB)
    target_compile_definitions(UVKBuildToolLib PRIVATE "UBT_LIB_COMPILE" "UVK_LIB_COMPILE" "YAML_CPP_DLL" "UBT_TARGET_WEB" "UVK_LOG_EXPORT_FROM_LIBRARY")
    target_compile_definitions(UVKBuildTool PRIVATE "UBT_TARGET_WEB" "UVK_LOG_EXPORT_FROM_LIBRARY")
else()
    target_compile_definitions(UVKBuildToolLib PRIVATE "UBT_LIB_COMPILE" "UVK_LIB_COMPILE" "YAML_CPP_DLL" "UBT_TARGET_FRAMEWORK" "UVK_LOG_EXPORT_FROM_LIBRARY")
    target_compile_definitions(UVKBuildTool PRIVATE "UBT_TARGET_FRAMEWORK" "UVK_LOG_EXPORT_FROM_LIBRARY")
endif()

target_link_libraries(UVKBuildToolLib yaml-cpp)
if (UBT_COMPILING_FOR_WEB)
    target_link_libraries(UVKBuildToolLib UntitledI18N)
endif()

target_link_libraries(UVKBuildTool UVKBuildToolLib)
