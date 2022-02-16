#include "SourceGenerator.hpp"

void UBT::generateGame(const char* name)
{
    auto game = std::ofstream(path + static_cast<std::string>("Source/Game.hpp"));

    game << R"(// Do not edit this file! This file was generated by the UVKBuildTool and will be overridden the next time you run regenerate!
#pragma once
#include "Engine/Engine.hpp"

#ifdef _WIN32
    #ifdef UVK_LIB_COMPILE
        #define )" << toUpper(name) << R"(_PUBLIC_API __declspec(dllexport)
    #else
        #define )" << toUpper(name) << R"(_PUBLIC_API __declspec(dllimport)
    #endif
#else
    #define )" << toUpper(name) << R"(_PUBLIC_API
#endif)";

    game.close();
}

void UBT::generateMain(const char* startupLevelName, const char* gameName)
{
    auto main = std::ofstream(path + static_cast<std::string>("Generated/main.cpp"));
    main << R"(// This is an autogenerated file, touching it is not recommended
#include <Engine.hpp>
#include "Source/StartupLevel.hpp"
#include "Source/)" << gameName << R"(GameInstance.hpp"
#include <GameFramework/Components/Components.hpp>
#include <WrapperSource/Wrapper.hpp>

int main(int argc, char** argv)
{
    ENABLE_FAST_IO(true);
    Wrapper::wbegin();
    UVK::AudioManager manager;

    bool bUsesEditor = false;
#ifndef PRODUCTION
    if (argv[1])
    {
        std::string cmd = argv[1];

        if (cmd == "--editor")
        {
            bUsesEditor = true;
        }
    }
#endif
    auto* st = new UVK::StartupLevel;
    UVK::Utility::getGlobal().getEditor() = bUsesEditor;
    UVK::Utility::getGlobal().currentLevel = st;
    auto* mode = new UVK::)" << gameName << R"(GameInstance();
    UVK::Utility::getGlobal().instance = mode;
    UVK::UVKGlobal::openLevelInternal(")" << startupLevelName << R"(", true);
    UVK::Renderer(UVK::Utility::getGlobal().currentLevel, bUsesEditor);
    Wrapper::wend();
})" << std::endl;
    main.close();

    main = std::ofstream(path + "Generated/mainmodded.cpp");
    main << R"(// This is an autogenerated file, touching it is not recommended
#include <Engine.hpp>
#include "Source/StartupLevel.hpp"
#include "Source/)" << gameName << R"(GameInstance.hpp"
#include <GameFramework/Components/Components.hpp>
#include <WrapperSource/Wrapper.hpp>
#include <urll.h>

int main(int argc, char** argv)
{
    ENABLE_FAST_IO(true);
    // load modded symbols
#ifdef _WIN32
    void* handle = URLL::dlopen("Modlib.dll");
#else
    // That ./ is required on unix systems
    void* handle = URLL::dlopen("./libModlib.so");
#endif
    bool bCanClose = false;
    if (handle != nullptr)
    {
        bCanClose = true;
        if (URLL::dlsym(handle, "modlibbegin", UVK::Utility::getGlobal().modbegin) == handle && URLL::dlsym(handle, "modlibend", UVK::Utility::getGlobal().modend) == handle && URLL::dlsym(handle, "modlibtick", UVK::Utility::getGlobal().modtick) == handle)
            logger.consoleLog("Loaded all mods!", UVK_LOG_TYPE_SUCCESS);
        else
            logger.consoleLog("Failed to load some or all of the initial mod library functions, mod events will not be loaded! Error: ", UVK_LOG_TYPE_WARNING, URLL::dlerror());
    }
    else
        logger.consoleLog("Failed to load the mod library!", UVK_LOG_TYPE_WARNING);
    Wrapper::wbegin();
    UVK::AudioManager manager;

    bool bUsesEditor = false;
#ifndef PRODUCTION
    if (argv[1])
    {
        std::string cmd = argv[1];

        if (cmd == "--editor")
            bUsesEditor = true;
    }
#endif
    auto* st = new UVK::StartupLevel;
    UVK::Utility::getGlobal().getEditor() = bUsesEditor;
    UVK::Utility::getGlobal().currentLevel = st;
    auto* mode = new UVK::)" << gameName << R"(GameInstance();
    UVK::Utility::getGlobal().instance = mode;
    UVK::UVKGlobal::openLevelInternal(")" << startupLevelName << R"(", true);
    UVK::Renderer(UVK::Utility::getGlobal().currentLevel, bUsesEditor);
    Wrapper::wend();
    if (bCanClose && URLL::dlclose(handle) != 0)
        logger.consoleLog("Error when closing the mod handle, message:", UVK_LOG_TYPE_ERROR, URLL::dlerror());
})" << std::endl;
    main.close();
}

void UBT::generateDef()
{
    std::ofstream out2(path + "Generated/BuildDef.hpp");
    out2 << "// Generated file, DO NOT TOUCH!" << std::endl;
    out2 << "#undef PRODUCTION" << std::endl;
    out2.close();
}

void UBT::generateWrapperAndMod()
{
    std::ofstream out(path + "WrapperSource/Wrapper.hpp");
    out << R"(#pragma once
#include <Generated/BuildDef.hpp>

namespace Wrapper
{
    void wbegin();
    void wend();
})" << std::endl;
    out.close();

    out = std::ofstream(path + "WrapperSource/Wrapper.cpp");
    out << R"(#include "Wrapper.hpp"

void Wrapper::wbegin()
{
}

void Wrapper::wend()
{
})" << std::endl;

    out.close();

    out = std::ofstream(path + "Generated/ModEmpty.hpp");
    out << R"(#pragma once
#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    EXPORT void modlibbegin();
    EXPORT void modlibtick(float deltaTime);
    EXPORT void modlibend();
#ifdef __cplusplus
}
#endif)" << std::endl;
    out.close();

    out = std::ofstream(path + "Generated/ModEmpty.cpp");
    out << R"(#include "ModEmpty.hpp"

void modlibbegin()
{
}

void modlibtick(float deltaTime)
{
}

void modlibend()
{
})" << std::endl;
    out.close();
}
