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
        stream << "set(CMAKE_CXX_STANDARD 17)" << std::endl;
        stream << "if(WIN32)" << std::endl;
        stream << "execute_process(COMMAND powershell \"cd Engine/ThirdParty/glew/auto && mingw32-make && cd .. && mingw32-make && ../../../\")" << std::endl;
        stream << "else()" << std::endl;
        stream << "execute_process(COMMAND bash \"cd Engine/ThirdParty/glew/auto && make && cd .. && make && ../../../\")" << std::endl;
        stream << "add_subdirectory(Engine/ThirdParty/glm/)" << std::endl;
        stream << "add_subdirectory(Engine/ThirdParty/glfw/)" << std::endl;
        stream << "add_subdirectory(Engine/ThirdParty/vulkan/1.2.162.1/source/Vulkan-Headers/)" << std::endl;
        stream << "if(MINGW)" << std::endl;
        stream << "else()" << std::endl;
        stream << "add_subdirectory(Engine/ThirdParty/openal/)" << std::endl;
        stream << "endif()" << std::endl;
        stream << "add_subdirectory(Engine/ThirdParty/yaml/)" << std::endl;
        stream << "add_subdirectory(Engine/ThirdParty/entt/)" << std::endl;
        stream << "link_directories(Engine/ThirdParty/glew/lib)" << std::endl;
        stream << "if(WIN32)" << std::endl;
        stream << "    link_directories(Engine/ThirdParty/vulkan/windows/)" << std::endl;
        stream << "elseif(Linux)" << std::endl;
        stream << "link_directories(Engine/ThirdParty/vulkan/1.2.162.1/x86_64/lib/)" << std::endl;
        stream << "endif()" << std::endl;
        stream << "include_directories(.)" << std::endl;
        stream << "include_directories(Engine/)" << std::endl;
        stream << "include_directories(Engine/ThirdParty/entt/src/entt)" << std::endl;
        stream << "include_directories(Engine/Core/)" << std::endl;
        stream << "include_directories(Engine/ThirdParty/glm)" << std::endl;
        stream << "include_directories(Engine/ThirdParty/)" << std::endl;
        stream << "include_directories(Engine/ThirdParty/glew/include)" << std::endl;
        stream << "include_directories(Engine/ThirdParty/yaml/include)" << std::endl;
        stream << "include_directories(Engine/ThirdParty/yaml/include/yaml-cpp)" << std::endl;
        stream << "include_directories(Engine/ThirdParty/logger/)" << std::endl;
        stream << "include_directories(Engine/ThirdParty/imgui/)" << std::endl;
        stream << "include_directories(Engine/ThirdParty/glfw/include/GLFW/)" << std::endl;
        stream << "include_directories(Engine/ThirdParty/vulkan/1.2.162.1/source/Vulkan-Headers/include/vulkan/)" << std::endl;
        stream << "include_directories(Engine/ThirdParty/stb/)" << std::endl;
        stream << "file(GLOB_RECURSE" <<  name << "Src \"Source/*.cpp\" \"Source/*.hpp\" \"Engine/Audio/*.cpp\" \"Engine/Audio/*.hpp\" \"Engine/Core/*.cpp\" \"Engine/Core/*.hpp\" \"Engine/Renderer/*.hpp\" \"Engine/Renderer/*.cpp\" \"Engine/GameFramework/*.cpp\" \"Engine/GameFramework/*.hpp\" \"Engine/ThirdParty/imgui/*.cpp\" \"Engine/ThirdParty/imgui/*.h\" \"Engine/ThirdParty/stb/*.h\")" << std::endl;
        stream << "add_executable(Game ${" << name << "Src} Engine/ThirdParty/logger/UVKLog.h main.cpp)" << std::endl;
        stream << "set_target_properties(Game PROPERTIES LINKER_LANGUAGE CXX)" << std::endl;
        stream << "if (WIN32)" << std::endl;
        stream << "    if (MINGW)" << std::endl;
        stream << "        target_link_libraries(" << name << " pthread glfw opengl32 glew32 yaml-cpp vulkan-1)" << std::endl;
        stream << "    else()" << std::endl;
        stream << "        target_link_libraries(" << name << " pthread glfw OpenAL opengl32 glew32 yaml-cpp vulkan-1)" << std::endl;
        stream << "    endif()" << std::endl;
        stream << "else()" << std::endl;
        stream << "    target_link_libraries(" << name << " glfw OpenGL GLEW OpenAL yaml-cpp vulkan)" << std::endl;
        stream << "endif ()" << std::endl;

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