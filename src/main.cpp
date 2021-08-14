#include "Utility.h"
#include "ActorListGenerator.hpp"
#include "CMakeGenerator.hpp"
#include "FileGenerator.hpp"
#include "SourceGenerator.hpp"
#include "ReleaseBuild.hpp"


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Not enough arguments passed into the generator!" << std::endl;
        return 0;
    }
    if (argv[1] == UBT::toLower("--help"))
    {
        std::cout << "UVKBuildTool - The universal Untitled Vulkan Game Engine file generator" << std::endl;
        std::cout << "  --generate - generate autogenerated files for your project" << std::endl;
	    std::cout << "  --install - generates first time install project files" << std::endl;
        std::cout << "  --help - this help message" << std::endl;
        std::cout << "Commands bellow this line, generate project files, with the first argument specifying the type and the second the name!" << std::endl;
        std::cout << "  --actor <name> <add or remove>" << std::endl;
        std::cout << "  --pawn <name>" << std::endl;
        std::cout << "  --game-mode <name>" << std::endl;
        std::cout << "  --game-state <name>" << std::endl;
        std::cout << "  --player-state <name>" << std::endl;
        std::cout << "  --player-controller <name>" << std::endl;
        std::cout << "  --level <name>" << std::endl;
        std::cout << "UVKBuild tool, made by MadLad Squad, developed and maintained by Stanislav Vasilev(Madman10K)" << std::endl;
        return 0;
    }

    YAML::Node config;

    try
    {
        config = YAML::LoadFile("../../uvproj.yaml");
    }
    catch (YAML::BadFile&)
    {
        std::cout << "Could not locate file" << std::endl;
    }

    if (argv[1] == UBT::toLower("--generate") && argc < 3)
    {
        bool bSetReadable;
        std::string name;
        std::string startupLevelName;

        std::ifstream i(path + "Generated/ActorList.hpp");
        bSetReadable = i.is_open();

        if (config["startup-level"])
        {
            startupLevelName = config["startup-level"].as<std::string>();
        }

        if (config["name"])
        {
            name = config["name"].as<std::string>();
        }

        if (!bSetReadable) UBT::generateSet();
        UBT::generateCmake(name.c_str());
        UBT::generateGame();
        UBT::generateMain(startupLevelName.c_str(), name.c_str());
        UBT::generateDef();

        return 0;
    }
    else if (argv[1] == UBT::toLower("--install") && argc < 3)
    {
        bool bSetReadable;
        std::string name;
        std::string startupLevelName;

        std::ifstream i(path + "Generated/ActorList.hpp");
        bSetReadable = i.is_open();

        if (config["startup-level"])
        {
            startupLevelName = config["startup-level"].as<std::string>();
        }

        if (config["name"])
        {
            name = config["name"].as<std::string>();
        }

        if (!bSetReadable) UBT::generateSet();
        UBT::generateCmake(name.c_str());
        UBT::generateGame();
        UBT::generateMain(startupLevelName.c_str(), name.c_str());
        UBT::generateDef();
        UBT::makeTemplate("StartupLevel", "UVK::Level");
		UBT::makeTemplate(static_cast<std::string>(name + std::string("GameInstance")), "UVK::GameInstance");

		return 0;
    }

    if (argc < 3)
    {
        std::cout << "Not enough arguments passed into the generator!" << std::endl;
        return 0;
    }
    else
    {
        if (argv[1] == UBT::toLower("--pawn"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::APawn");
            return 0;
        }
        else if (argv[1] == UBT::toLower("--game-mode"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::GameMode");
            return 0;
        }
        else if (argv[1] == UBT::toLower("--game-state"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::GameState");
            return 0;
        }
        else if (argv[1] == UBT::toLower("--player-state"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::PlayerState");
            return 0;
        }
        else if (argv[1] == UBT::toLower("--player-controller"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::PlayerController");
            return 0;
        }
        else if (argv[1] == UBT::toLower("--level"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::Level");
            return 0;
        }
        else if (argv[1] == UBT::toLower("--build"))
        {
            UBT::relBuild(std::atoi(argv[2]));
            return 0;
        }
    }

    if (argc < 4)
    {
        std::cout << "Not enough arguments passed into the generator!" << std::endl;
        return 0;
    }
    else
    {
        if (argv[1] == UBT::toLower("--actor") && argv[3] == UBT::toLower("--add"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::ScriptableObject");
            UBT::addClass("Source/" + std::string(argv[2]) + ".hpp");
            return 0;
        }
        else if (argv[1] == UBT::toLower("--actor") && argv[3] == UBT::toLower("--remove"))
        {
            UBT::removeClass("Source/" + std::string(argv[2]) + ".hpp");
            return 0;
        }

    }
}
