#pragma once
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <algorithm>
#include <fstream>
#include <ostream>

namespace UBT
{
    std::string toLower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    void GenerateFiles(int subsystem, bool bIsInProduction, const char* name)
    {
        auto stream = std::ofstream("../../CMakeLists.txt");
        stream << "cmake_minimum_required(VERSION 3.17)" << std::endl;
        stream << "project(" << name << ")" << std::endl;
        stream << R"(set(CMAKE_CXX_STANDARD 17)
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
include_directories(Engine/ThirdParty/glew/include)
include_directories(Engine/ThirdParty/yaml/include)
include_directories(Engine/ThirdParty/yaml/include/yaml-cpp)
include_directories(Engine/ThirdParty/logger/)
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
        stream << "    file(GLOB_RECURSE " << name << "Src" << R"( "Source/*.cpp" "Source/*.hpp" "Engine/Audio/*.cpp" "Engine/Audio/*.hpp" "Engine/Core/*.cpp" "Engine/Core/*.hpp" "Engine/Renderer/OpenGL/*.hpp" "Engine/Renderer/OpenGL/*.cpp" "Engine/Renderer/Vulkan/VulkanRenderer.hpp" "Engine/Renderer/Renderer.hpp" "Engine/GameFramework/*.cpp" "Engine/GameFramework/*.hpp" "Engine/ThirdParty/imgui/backends/apple/*.cpp" "Engine/ThirdParty/imgui/backends/apple/*.h" "Engine/ThirdParty/imgui/misc/*.cpp" "Engine/ThirdParty/imgui/misc/*.h"))" << std::endl;
        stream << "else()" << std::endl;
        stream << "    file(GLOB_RECURSE " << name << "Src" << R"( "Source/*.cpp" "Source/*.hpp" "Engine/Audio/*.cpp" "Engine/Audio/*.hpp" "Engine/Core/*.cpp" "Engine/Core/*.hpp" "Engine/Renderer/*.hpp" "Engine/Renderer/*.cpp" "Engine/GameFramework/*.cpp" "Engine/GameFramework/*.hpp" "Engine/ThirdParty/imgui/backends/everything/*.cpp" "Engine/ThirdParty/imgui/misc/*.cpp" "Engine/ThirdParty/imgui/misc/*.h" "Engine/ThirdParty/imgui/backends/everything/*.h" "Engine/ThirdParty/imgui/backends/Vulkan/*.h" "Engine/ThirdParty/imgui/backends/Vulkan/*.cpp"))" << std::endl;
        stream << "endif ()" << std::endl;
        stream << std::endl;
        stream << "if(WIN32)" << std::endl;
        stream << "    add_executable("<< name << " ${"<< name << "Src} Engine/ThirdParty/logger/UVKLog.h Engine/ThirdParty/stb/stb_image.h Engine/ThirdParty/stb/sndfile.h main.cpp )" << std::endl;
        stream << "else()" << std::endl;
        stream << "    add_executable("<< name << " ${"<< name << "Src} Engine/ThirdParty/logger/UVKLog.h Engine/ThirdParty/stb/stb_image.h main.cpp )" << std::endl;
        stream << "endif()" << std::endl;
        stream << "set_target_properties("<< name << " PROPERTIES LINKER_LANGUAGE CXX)" << std::endl;
        stream << "if (WIN32)" << std::endl;
        stream << "    if (MINGW)" << std::endl;
        stream << "        target_link_libraries("<< name << " glfw opengl32 glew32 yaml-cpp vulkan-1)" << std::endl;
        stream << "    else()" << std::endl;
        stream << "        target_link_libraries("<< name << " glfw OpenAL opengl32 glew32 yaml-cpp vulkan-1 sndfile)" << std::endl;
        stream << "    endif()" << std::endl;
        stream << "elseif(APPLE)" << std::endl;
        stream << "    target_link_libraries("<< name << " glfw ${OPENGL_LIBRARIES} OpenAL yaml-cpp sndfile)" << std::endl;
        stream << "else()" << std::endl;
        stream << "    target_link_libraries("<< name << " glfw GLEW OpenGL OpenAL yaml-cpp vulkan sndfile)" << std::endl;
        stream << "endif()" << std::endl;

        stream.close();

        auto stream2 = std::ofstream("../../Source/Game.hpp");

        stream2 << "#include \"Engine/Engine.hpp\"" << std::endl;

        switch(subsystem)
        {
            case 1:
                stream2 << std::endl;
                break;
            case 2:
                stream2 << "#undef Legacy" << std::endl;
                break;
            default:
                stream2 << std::endl;
        }

        if(bIsInProduction)
                stream2 << "#define Production" << std::endl;

        stream2.close();
    }

    void GenActor(char* name)
    {
        auto stream = std::ofstream("../../" + static_cast<std::string>(name) + ".cpp");

        stream << "#include " << "\"" << name << ".hpp\"" << std::endl;
        stream << std::endl;
        stream << "UVK::" << name << "::" << name << "()" << std::endl;
        stream << "{" << std::endl;
        stream << "     auto& actor = registry.createActor(\"" << name << "\");" << std::endl;
        stream << "}" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::beginPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::tick(float deltaTime)" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << "void UVK::" << name << "::endPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream.close();

        auto stream2 = std::ofstream("../../" + static_cast<std::string>(name) + ".hpp");
        stream2 << "#include \"Game.hpp\"" << std::endl;
        stream2 << std::endl;
        stream2 << "namespace UVK" << std::endl;
        stream2 << "{" << std::endl;
        stream2 << "    class " << name << " : UVK::ScriptableObject" << std::endl;
        stream2 << "    {" << std::endl;
        stream2 << "    public:" << std::endl;
        stream2 << "        " << name << "();" << std::endl;
        stream2 << "        virtual void tick(float deltaTime) override;" << std::endl;
        stream2 << "        virtual void endPlay() override;" << std::endl;
        stream2 << "        virtual void beginPlay() override;" << std::endl;
        stream2 << "    };" << std::endl;
        stream2 << "}" << std::endl;
        stream2.close();
    }

    void GenPawn(char* name)
    {
        auto stream = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".cpp");

        stream << "#include " << "\"" << name << ".hpp\"" << std::endl;
        stream << std::endl;
        stream << "UVK::" << name << "::" << name << "()" << std::endl;
        stream << "{" << std::endl;
        stream << "     auto& actor = registry.createActor(\"" << name << "\");" << std::endl;
        stream << "}" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::beginPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::tick(float deltaTime)" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << "void UVK::" << name << "::endPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream.close();

        auto stream2 = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".hpp");
        stream2 << "#include \"Game.hpp\"" << std::endl;
        stream2 << std::endl;
        stream2 << "namespace UVK" << std::endl;
        stream2 << "{" << std::endl;
        stream2 << "    class " << name << " : UVK::APawn" << std::endl;
        stream2 << "    {" << std::endl;
        stream2 << "    public:" << std::endl;
        stream2 << "        " << name << "();" << std::endl;
        stream2 << "        virtual void tick(float deltaTime) override;" << std::endl;
        stream2 << "        virtual void endPlay() override;" << std::endl;
        stream2 << "        virtual void beginPlay() override;" << std::endl;
        stream2 << "    };" << std::endl;
        stream2 << "}" << std::endl;
        stream2.close();
    }

    void GenGM(char* name)
    {
        auto stream = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".cpp");

        stream << "#include " << "\"" << name << ".hpp\"" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::beginPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::tick(float deltaTime)" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << "void UVK::" << name << "::endPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream.close();

        auto stream2 = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".hpp");
        stream2 << "#include \"Game.hpp\"" << std::endl;
        stream2 << std::endl;
        stream2 << "namespace UVK" << std::endl;
        stream2 << "{" << std::endl;
        stream2 << "    class " << name << " : UVK::GameMode" << std::endl;
        stream2 << "    {" << std::endl;
        stream2 << "    public:" << std::endl;
        stream2 << "        " << name << "();" << std::endl;
        stream2 << "        virtual void tick(float deltaTime) override;" << std::endl;
        stream2 << "        virtual void endPlay() override;" << std::endl;
        stream2 << "        virtual void beginPlay() override;" << std::endl;
        stream2 << "    };" << std::endl;
        stream2 << "}" << std::endl;
        stream2.close();
    }

    void GenGS(char* name)
    {
        auto stream = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".cpp");

        stream << "#include " << "\"" << name << ".hpp\"" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::beginPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::tick(float deltaTime)" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << "void UVK::" << name << "::endPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream.close();

        auto stream2 = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".hpp");
        stream2 << "#include \"Game.hpp\"" << std::endl;
        stream2 << std::endl;
        stream2 << "namespace UVK" << std::endl;
        stream2 << "{" << std::endl;
        stream2 << "    class " << name << " : UVK::GameState" << std::endl;
        stream2 << "    {" << std::endl;
        stream2 << "    public:" << std::endl;
        stream2 << "        " << name << "();" << std::endl;
        stream2 << "        virtual void tick(float deltaTime) override;" << std::endl;
        stream2 << "        virtual void endPlay() override;" << std::endl;
        stream2 << "        virtual void beginPlay() override;" << std::endl;
        stream2 << "    };" << std::endl;
        stream2 << "}" << std::endl;
        stream2.close();
    }

    void GenPS(char* name)
    {
        auto stream = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".cpp");

        stream << "#include " << "\"" << name << ".hpp\"" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::beginPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::tick(float deltaTime)" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << "void UVK::" << name << "::endPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream.close();

        auto stream2 = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".hpp");
        stream2 << "#include \"Game.hpp\"" << std::endl;
        stream2 << std::endl;
        stream2 << "namespace UVK" << std::endl;
        stream2 << "{" << std::endl;
        stream2 << "    class " << name << " : UVK::PlayerState" << std::endl;
        stream2 << "    {" << std::endl;
        stream2 << "    public:" << std::endl;
        stream2 << "        " << name << "();" << std::endl;
        stream2 << "        virtual void tick(float deltaTime) override;" << std::endl;
        stream2 << "        virtual void endPlay() override;" << std::endl;
        stream2 << "        virtual void beginPlay() override;" << std::endl;
        stream2 << "    };" << std::endl;
        stream2 << "}" << std::endl;
        stream2.close();
    }

    void GenPC(char* name)
    {
        auto stream = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".cpp");

        stream << "#include " << "\"" << name << ".hpp\"" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::beginPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::tick(float deltaTime)" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << "void UVK::" << name << "::endPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream.close();

        auto stream2 = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".hpp");
        stream2 << "#include \"Game.hpp\"" << std::endl;
        stream2 << std::endl;
        stream2 << "namespace UVK" << std::endl;
        stream2 << "{" << std::endl;
        stream2 << "    class " << name << " : UVK::PlayerController" << std::endl;
        stream2 << "    {" << std::endl;
        stream2 << "    public:" << std::endl;
        stream2 << "        " << name << "();" << std::endl;
        stream2 << "        virtual void tick(float deltaTime) override;" << std::endl;
        stream2 << "        virtual void endPlay() override;" << std::endl;
        stream2 << "        virtual void beginPlay() override;" << std::endl;
        stream2 << "    };" << std::endl;
        stream2 << "}" << std::endl;
        stream2.close();
    }

    void GenGI(char* name)
    {
        auto stream = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".cpp");

        stream << "#include " << "\"" << name << ".hpp\"" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::beginPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::tick(float deltaTime)" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << "void UVK::" << name << "::endPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream.close();

        auto stream2 = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".hpp");
        stream2 << "#include \"Game.hpp\"" << std::endl;
        stream2 << std::endl;
        stream2 << "namespace UVK" << std::endl;
        stream2 << "{" << std::endl;
        stream2 << "    class " << name << " : UVK::GameInstance" << std::endl;
        stream2 << "    {" << std::endl;
        stream2 << "    public:" << std::endl;
        stream2 << "        " << name << "();" << std::endl;
        stream2 << "        virtual void tick(float deltaTime) override;" << std::endl;
        stream2 << "        virtual void endPlay() override;" << std::endl;
        stream2 << "        virtual void beginPlay() override;" << std::endl;
        stream2 << "    };" << std::endl;
        stream2 << "}" << std::endl;
        stream2.close();
    }

    void GenLVL(char* name)
    {
        auto stream = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".cpp");

        stream << "#include " << "\"" << name << ".hpp\"" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::beginPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << std::endl;
        stream << "void UVK::" << name << "::tick(float deltaTime)" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream << "void UVK::" << name << "::endPlay()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
        stream.close();

        auto stream2 = std::ofstream("../../Source/" + static_cast<std::string>(name) + ".hpp");
        stream2 << "#include \"Game.hpp\"" << std::endl;
        stream2 << std::endl;
        stream2 << "namespace UVK" << std::endl;
        stream2 << "{" << std::endl;
        stream2 << "    class " << name << " : UVK::Level" << std::endl;
        stream2 << "    {" << std::endl;
        stream2 << "    public:" << std::endl;
        stream2 << "        " << name << "();" << std::endl;
        stream2 << "        virtual void tick(float deltaTime) override;" << std::endl;
        stream2 << "        virtual void endPlay() override;" << std::endl;
        stream2 << "        virtual void beginPlay() override;" << std::endl;
        stream2 << "    };" << std::endl;
        stream2 << "}" << std::endl;
        stream2.close();
    }
}
