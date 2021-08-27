#include "CMakeGenerator.hpp"

void UBT::generateCmake(const char* name)
{
    auto stream = std::ofstream(path + static_cast<std::string>("CMakeLists.txt"));
    stream << "cmake_minimum_required(VERSION 3.17)" << std::endl;
    stream << "project(" << name << ")" << std::endl;
    stream << R"(set(CMAKE_CXX_STANDARD 20)
add_subdirectory(Engine/ThirdParty/glm/)
add_subdirectory(Engine/ThirdParty/glfw/)
if(APPLE)
        else()
add_subdirectory(Engine/ThirdParty/vulkan/headers/)
endif()
if(MINGW)
else()
    add_subdirectory(Engine/ThirdParty/openal/)
endif()
add_subdirectory(Engine/ThirdParty/yaml/)
add_subdirectory(Engine/ThirdParty/entt/)
link_directories(Engine/ThirdParty/glew/lib)
add_subdirectory(Engine/ThirdParty/assimp)
include_directories(Engine/ThirdParty/assimp/include)
if(APPLE)
else()
    link_directories(Engine/ThirdParty/vulkan/)
    link_directories(Engine/ThirdParty/vulkan/lib)
endif()
include_directories(.)
include_directories(Engine/)
include_directories(Engine/ThirdParty/entt/src/entt)
include_directories(Engine/Core/)
include_directories(Engine/ThirdParty/glm)
include_directories(Engine/ThirdParty/)
include_directories(Engine/ThirdParty/imguiex/imguizmo)
include_directories(Engine/ThirdParty/glew/include)
include_directories(Engine/ThirdParty/yaml/include)
include_directories(Engine/ThirdParty/yaml/include/yaml-cpp)
include_directories(Engine/ThirdParty/logger/)
include_directories(Source/)
if(APPLE)
    include_directories(Engine/ThirdParty/imgui/backends/apple/)
else()
    include_directories(Engine/ThirdParty/imgui/backends/everything/)
    include_directories(Engine/ThirdParty/imgui/backends/Vulkan/)
endif()
include_directories(Engine/ThirdParty/imgui/misc/)
include_directories(Engine/ThirdParty/glfw/include/GLFW/)
if (APPLE)
else()
    include_directories(Engine/ThirdParty/vulkan/headers/include)
endif ()
if(WIN32)
    include_directories(Engine/ThirdParty/stb)
else()
    include_directories(Engine/ThirdParty)
endif()
if (APPLE))" << std::endl;
    stream << "    file(GLOB_RECURSE " << name << "Src" << R"( "Source/*.cpp" "Source/*.hpp" "Engine/Audio/*.cpp" "Engine/Audio/*.hpp" "Engine/Core/*.cpp" "Engine/Core/*.hpp" "Engine/Renderer/OpenGL/*.hpp" "Engine/Renderer/OpenGL/*.cpp" "Engine/Renderer/Vulkan/VulkanRenderer.hpp" "Engine/Renderer/Renderer.hpp" "Engine/GameFramework/*.cpp" "Engine/GameFramework/*.hpp" "Engine/ThirdParty/imgui/backends/apple/*.cpp" "Engine/ThirdParty/imgui/backends/apple/*.h" "Engine/ThirdParty/imgui/misc/*.cpp" "Engine/ThirdParty/imgui/misc/*.h" "Engine/ThirdParty/imguiex/*.cpp" "Engine/ThirdParty/imguiex/*.h" "Generated/*.cpp" "Generated/*.hpp"))" << std::endl;
    stream << "else()" << std::endl;
    stream << "    file(GLOB_RECURSE " << name << "Src" << R"( "Source/*.cpp" "Source/*.hpp" "Engine/Audio/*.cpp" "Engine/Audio/*.hpp" "Engine/Core/*.cpp" "Engine/Core/*.hpp" "Engine/Renderer/*.hpp" "Engine/Renderer/*.cpp" "Engine/GameFramework/*.cpp" "Engine/GameFramework/*.hpp" "Engine/ThirdParty/imgui/backends/everything/*.cpp" "Engine/ThirdParty/imgui/misc/*.cpp" "Engine/ThirdParty/imgui/misc/*.h" "Engine/ThirdParty/imgui/backends/everything/*.h" "Engine/ThirdParty/imgui/backends/Vulkan/*.h" "Engine/ThirdParty/imgui/backends/Vulkan/*.cpp" "Engine/ThirdParty/imguiex/*.cpp" "Engine/ThirdParty/imguiex/*.h" "Generated/*.cpp" "Generated/*.hpp"))" << std::endl;
    stream << "endif ()" << std::endl;
    stream << std::endl;
    stream << "if(WIN32)" << std::endl;
    stream << "    add_executable("<< name << " ${"<< name << "Src} Engine/ThirdParty/logger/UVKLog.h Engine/ThirdParty/stb/stb_image.h Engine/ThirdParty/stb/sndfile.h)" << std::endl;
    stream << "else()" << std::endl;
    stream << "    add_executable("<< name << " ${"<< name << "Src} Engine/ThirdParty/logger/UVKLog.h Engine/ThirdParty/stb/stb_image.h)" << std::endl;
    stream << "endif()" << std::endl;
    stream << "set_target_properties("<< name << " PROPERTIES LINKER_LANGUAGE CXX)" << std::endl;
    stream << "if (WIN32)" << std::endl;
    stream << "    if (MINGW)" << std::endl;
    stream << "        target_compile_options("<< name << " PRIVATE \"-O3\" \"-march=native\")" << std::endl;
    stream << "        target_link_libraries("<< name << " glfw opengl32 glew32 yaml-cpp vulkan-1 assimp)" << std::endl;
    stream << "    else()" << std::endl;
    stream << "        target_compile_options("<< name << " PRIVATE \"/O2bi\" \"/arch:AVX2\")" << std::endl;
    stream << "        target_link_libraries("<< name << " glfw OpenAL opengl32 glew32 yaml-cpp vulkan-1 sndfile assimp)" << std::endl;
    stream << "    endif()" << std::endl;
    stream << "elseif(APPLE)" << std::endl;
    stream << "    target_compile_options("<< name << " PRIVATE \"-O3\" \"-march=native\")" << std::endl;
    stream << "    target_link_libraries("<< name << " glfw GLEW ${OPENGL_LIBRARIES} OpenAL yaml-cpp sndfile assimp)" << std::endl;
    stream << "else()" << std::endl;
    stream << "    target_compile_options("<< name << " PRIVATE \"-O3\" \"-march=native\")" << std::endl;
    stream << "    target_link_libraries("<< name << " glfw GLEW OpenGL OpenAL yaml-cpp vulkan sndfile util pthread assimp)" << std::endl;
    stream << "endif()" << std::endl;

    stream.close();
}
