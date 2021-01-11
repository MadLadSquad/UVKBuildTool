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
	stream << "find_package(OpenGL REQUIRED)" << std::endl;
        stream << "add_subdirectory(Engine/ThirdParty/glm/)" << std::endl;
        stream << "add_subdirectory(Engine/ThirdParty/glfw/)" << std::endl;
        stream << "if(MINGW)" << std::endl;
        stream << "else()" << std::endl;
        stream << "     add_subdirectory(Engine/ThirdParty/openal/)" << std::endl;
        stream << "endif()" << std::endl;
        stream << "add_subdirectory(Engine/ThirdParty/yaml/)" << std::endl;
        stream << "add_subdirectory(Engine/ThirdParty/entt/)" << std::endl;

        stream << "link_directories(Engine/ThirdParty/glew/lib)" << std::endl;

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

        stream << "file(GLOB_RECURSE " << name << "Src \"Source/*.cpp\" \"Source/*.hpp\" \"Engine/Audio/*.cpp\" \"Engine/Audio/*.hpp\" \"Engine/Core/*.cpp\" \"Engine/Core/*.hpp\" \"Engine/Renderer/*.hpp\" \"Engine/Renderer/*.cpp\" \"Engine/GameFramework/*.cpp\" \"Engine/GameFramework/*.hpp\")" << std::endl;
        stream << "add_executable(" << name << " ${" << name << "Src} Engine/ThirdParty/logger/UVKLog.h main.cpp)" << std::endl;
        stream << "set_target_properties(" << name << " PROPERTIES LINKER_LANGUAGE CXX)" << std::endl;

        stream << "if (WIN32)" << std::endl;
        stream << "     if (MINGW)" << std::endl;
        stream << "         target_link_libraries(" << name << " pthread glfw opengl32 glew32 yaml-cpp)" << std::endl;
        stream << "     else()" << std::endl;
        stream << "         target_link_libraries(" << name << " pthread glfw OpenAL opengl32 glew32 yaml-cpp)" << std::endl;
        stream << "     endif()" << std::endl;
        stream << "elseif(APPLE)" << std::endl;
        stream << "     target_link_libraries(" << name << " pthread glfw ${OPENGL_LIBRARIES} OpenAL GLEW yaml-cpp)" << std::endl;
        stream << "else()" << std::endl;
        stream << "     target_link_libraries(" << name << " glfw OpenGL GLEW OpenAL yaml-cpp)" << std::endl;
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
}
