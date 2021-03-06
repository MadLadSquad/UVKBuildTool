#include "SourceGenerator.hpp"

void UBT::generateGame()
{
    auto game = std::ofstream(path + static_cast<std::string>("Source/Game.hpp"));

    game << "// Do not edit this file! This file was generated by the UVKBuildTool and will be overridden the next time you run regenerate!" << std::endl;
    game << "#include \"Engine/Engine.hpp\"" << std::endl;
    game.close();
}

void UBT::generateMain(const char* startupLevelName)
{
    auto main = std::ofstream(path + static_cast<std::string>("Generated/main.cpp"));
    main << R"(// This is an autogenerated file, touching it is not recommended
#include <Engine.hpp>
#include "Source/StartupLevel.hpp"

int main(int argc, char** argv)
{
    ENABLE_FAST_IO(true);
    UVK::AudioManager manager;
    bool bUsesEditor = false;

    if (argv[1])
    {
        std::string cmd = argv[1];

        if (cmd == "--editor")
        {
            bUsesEditor = true;
        }
    }
    
    auto* st = new UVK::StartupLevel;
    UVK::global.bEditor = bUsesEditor;
    UVK::global.level = st;
    
    UVK::StartupLevel::open(")" << startupLevelName << "\");" << std::endl;
    main << "    UVK::Renderer(UVK::global.level, bUsesEditor);" << std::endl;
    main << "}" << std::endl;
    main.close();
}

void UBT::generateDef()
{
    std::ofstream out2(path + "Generated/BuildDef.hpp");
    out2 << "// Generated file, DO NOT TOUCH!" << std::endl;
    out2 << "#undef PRODUCTION" << std::endl;
    out2.close();
}
