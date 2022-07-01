cmake_minimum_required(VERSION 3.17)

project(UVKBuildToolLib)
project(UVKBuildTool)

# This option is needed to define a compiler macro and the given source files to be used by the compilation target
option(UBT_COMPILING_FOR_ENGINE "If set to ON it will compile with configuration for the UntitledVulkanGameEngine, otherwise will compile for the UntitledImGuiFramework!" ON)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS ON)
add_subdirectory(yaml-cpp)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(utg/ThirdParty/utfcpp)

include_directories(yaml-cpp/include)
include_directories(yaml-cpp/include/yaml-cpp)
include_directories(utg/src/)
include_directories(utg/ThirdParty/utfcpp/source/)
include_directories(src/)

if (UBT_COMPILING_FOR_ENGINE)
    file(GLOB_RECURSE SRC "src/UntitledVulkanGameEngine/*.cpp" )
    file(GLOB_RECURSE HEAD "src/UntitledVulkanGameEngine/*.hpp" )
else()
    file(GLOB_RECURSE SRC "src/UntitledImGuiFramework/*.cpp" "utg/src/*.cpp")
    file(GLOB_RECURSE HEAD "src/UntitledImGuiFramework/*.hpp" "utg/src/*.hpp")
endif()
file(GLOB_RECURSE UNI_HEAD "src/Uniform/*.h" "utg/src/*.hpp")
file(GLOB_RECURSE UNI_SRC "src/Uniform/*.cpp")
file(GLOB_RECURSE UTG_SRC "utg/src/*.cpp")

add_library(UVKBuildToolLib SHARED ${SRC} ${HEAD} ${UNI_SRC} ${UNI_HEAD})
add_executable(UVKBuildTool ${HEAD} ${UNI_HEAD} main.cpp ${UTG_SRC})

set_target_properties(UVKBuildTool PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(UVKBuildToolLib PROPERTIES LINKER_LANGUAGE CXX)
if (UBT_COMPILING_FOR_ENGINE)
    target_compile_definitions(UVKBuildToolLib PRIVATE "UBT_LIB_COMPILE" "YAML_CPP_DLL" "UBT_TARGET_ENGINE")
    target_compile_definitions(UVKBuildTool PRIVATE "UBT_TARGET_ENGINE")
else()
    target_compile_definitions(UVKBuildToolLib PRIVATE "UBT_LIB_COMPILE" "YAML_CPP_DLL" "UBT_TARGET_FRAMEWORK")
    target_compile_definitions(UVKBuildTool PRIVATE "UBT_TARGET_FRAMEWORK")
endif()

target_link_libraries(UVKBuildToolLib yaml-cpp)
target_link_libraries(UVKBuildTool UVKBuildToolLib yaml-cpp)