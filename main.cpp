#ifdef UBT_TARGET_ENGINE
    #include <Uniform/UniformUtility.h>
    #include <UntitledVulkanGameEngine/ActorListGenerator.hpp>
    #include <UntitledVulkanGameEngine/CMakeGenerator.hpp>
    #include <UntitledVulkanGameEngine/FileGenerator.hpp>
    #include <UntitledVulkanGameEngine/SourceGenerator.hpp>
    #include <UntitledVulkanGameEngine/ReleaseBuild.hpp>
#else
    #include <UntitledImGuiFramework/Utility.hpp>
    #include <UntitledImGuiFramework/CMakeGenerator.hpp>
    #include <UntitledImGuiFramework/FileGenerator.hpp>
    #include <UntitledImGuiFramework/SourceGenerator.hpp>
    #include <UntitledImGuiFramework/ReleaseBuild.hpp>
#endif
#include <GeneratorCore.hpp>

void getConfig(YAML::Node& config, std::string& name)
{
    try
    {
        std::string tmp = UBT::getPath() + "uvproj.yaml";
        config = YAML::LoadFile(tmp);
    }
    catch (YAML::BadFile&)
    {
        std::cout << "Could not locate file" << std::endl;
        std::terminate();
    }

    if (config["name"])
    {
        name = config["name"].as<std::string>();
    }
}
#ifdef UBT_TARGET_ENGINE
int main(int argc, char** argv)
{
    UBT::setPath("../../");
    if (argc < 2)
    {
        std::cout << "Not enough arguments passed into the generator!" << std::endl;
        return 0;
    }
    if (argv[1] == UBT::toLower("--help"))
    {

        UTG::Input in;
        auto result = in.init("../Templates/HelpMessage.tmpl", UTG::Input::INPUT_TYPE_FILE);
        if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
        {
            std::cout << "\x1b[31mError when opening the HelpMessage.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
            std::terminate();
        }
        std::string buf;
        std::string tmp;
        std::ifstream instr("../Templates/UntitledVulkanGameEngine/ArgumentsRegistry.tmpl");
        while (std::getline(instr, tmp))
            buf += tmp + "\n";
        in["arguments"] = buf;
        std::cout << in.process() << std::endl;
        return 0;
    }

    YAML::Node config;
    std::string name;
    if (argv[1] == UBT::toLower("--generate") && argc < 4)
    {
        UBT::setPath(argv[2]);
        getConfig(config, name);
        bool bSetReadable;

        std::string startupLevelName;

        std::ifstream i(UBT::getPath() + "Generated/ActorList.hpp");
        bSetReadable = i.is_open();

        if (config["startup-level"])
        {
            startupLevelName = config["startup-level"].as<std::string>();
        }

        UBT::CMakeInfoData data;

        UBT::addIncludeDirectories(config, data);
        UBT::addSubdirectories(config, data);
        UBT::addLinkLibraries(config, data);
        UBT::addHeaderLibraries(config, data);
        UBT::addSourceLibraries(config, data);

        if (!bSetReadable) UBT::generateSet();
        UBT::generateCmake(name.c_str(), data);
        UBT::generateGame(name.c_str());
        UBT::generateMain(startupLevelName.c_str(), name.c_str());
        UBT::generateDef();

        return 0;
    }
    else if (argv[1] == UBT::toLower("--install") && argc < 4)
    {
        UBT::setPath(argv[2]);
        getConfig(config, name);
        bool bSetReadable;
        std::string startupLevelName;

        std::ifstream i(UBT::getPath() + "Generated/ActorList.hpp");
        bSetReadable = i.is_open();

        if (config["startup-level"])
        {
            startupLevelName = config["startup-level"].as<std::string>();
        }

        if (config["name"])
        {
            name = config["name"].as<std::string>();
        }

        UBT::CMakeInfoData data;

        UBT::addIncludeDirectories(config, data);
        UBT::addSubdirectories(config, data);
        UBT::addLinkLibraries(config, data);
        UBT::addHeaderLibraries(config, data);
        UBT::addSourceLibraries(config, data);

        if (!bSetReadable) UBT::generateSet();
        UBT::generateCmake(name.c_str(), data);
        UBT::generateGame(name.c_str());
        UBT::generateMain(startupLevelName.c_str(), name.c_str());
        UBT::generateDef();
        UBT::generateWrapperAndMod();
        UBT::makeTemplate("StartupLevel", "UVK::Level", name.c_str());
		UBT::makeTemplate(static_cast<std::string>(name + std::string("GameInstance")), "UVK::GameInstance", name.c_str());

		return 0;
    }
    else if (argv[1] == UBT::toLower("--build"))
    {
        UBT::setPath(argv[2]);
        getConfig(config, name);
        UBT::relBuild(config["name"].as<std::string>());
        return 0;
    }

    if (argc < 4)
    {
        std::cout << "Not enough arguments passed into the generator!" << std::endl;
        return 0;
    }
    else if (argc == 4)
    {
        UBT::setPath(argv[3]);
        getConfig(config, name);
        if (argv[1] == UBT::toLower("--pawn"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::Pawn", name.c_str());
            return 0;
        }
        else if (argv[1] == UBT::toLower("--game-mode"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::GameMode", name.c_str());
            return 0;
        }
        else if (argv[1] == UBT::toLower("--game-state"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::GameState", name.c_str());
            return 0;
        }
        else if (argv[1] == UBT::toLower("--player-state"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::PlayerState", name.c_str());
            return 0;
        }
        else if (argv[1] == UBT::toLower("--player-controller"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::PlayerController", name.c_str());
            return 0;
        }
        else if (argv[1] == UBT::toLower("--level"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::Level", name.c_str());
            return 0;
        }
    }

    if (argc < 5)
    {
        std::cout << "Not enough arguments passed into the generator!" << std::endl;
        return 0;
    }
    else if (argc == 5)
    {
        UBT::setPath(argv[4]);
        getConfig(config, name);
        if (argv[1] == UBT::toLower("--scriptable-object") && argv[3] == UBT::toLower("--add"))
        {
            UBT::makeTemplate(std::string(argv[2]), "UVK::ScriptableObject", name.c_str());
            UBT::addClass("Source/" + std::string(argv[2]) + ".hpp");
            return 0;
        }
        else if (argv[1] == UBT::toLower("--scriptable-object") && argv[3] == UBT::toLower("--remove"))
        {
            UBT::removeClass("Source/" + std::string(argv[2]) + ".hpp");
            return 0;
        }

    }
}
#else
int main(int argc, char** argv)
{
    UBT::setPath("../../");
    if (argc < 2)
    {
        std::cout << "Not enough arguments passed into the generator!" << std::endl;
        return 0;
    }
    if (argv[1] == UBT::toLower("--help"))
    {

        UTG::Input in;
        auto result = in.init("../Templates/HelpMessage.tmpl", UTG::Input::INPUT_TYPE_FILE);
        if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
        {
            std::cout << "\x1b[31mError when opening the HelpMessage.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
            std::terminate();
        }
        std::string buf;
        std::string tmp;
        std::ifstream instr("../Templates/UntitledImGuiFramework/ArgumentsRegistry.tmpl");
        while (std::getline(instr, tmp))
            buf += tmp + "\n";
        in["arguments"] = buf;
        std::cout << in.process() << std::endl;
        return 0;
    }

    YAML::Node config;
    std::string name;
    if (argv[1] == UBT::toLower("--generate") && argc < 4)
    {
        UBT::setPath(argv[2]);
        getConfig(config, name);

        std::string startupLevelName;
        std::ifstream i(UBT::getPath() + "Generated/ActorList.hpp");

        UBT::generateMain(name.c_str());
        UBT::generateDef();
        return 0;
    }
    else if (argv[1] == UBT::toLower("--install") && argc < 4)
    {
        UBT::setPath(argv[2]);
        getConfig(config, name);

        std::string startupLevelName;
        std::ifstream i(UBT::getPath() + "Generated/ActorList.hpp");
        if (config["name"])
            name = config["name"].as<std::string>();

        UBT::generateCmake(name.c_str());
        UBT::generateMain(name.c_str());
        UBT::generateDef();
        UBT::makeTemplate(static_cast<std::string>(name + std::string("UIInstance")), "Instance", name.c_str());
        return 0;
    }
    else if (argv[1] == UBT::toLower("--build"))
    {
        UBT::setPath(argv[2]);
        getConfig(config, name);
        UBT::relBuild(config["name"].as<std::string>());
        return 0;
    }

    if (argc < 4)
    {
        std::cout << "\x1b[31mError: Not enough arguments passed into the generator!\x1b[0m" << std::endl;
        return 0;
    }
    else if (argc == 4)
    {
        UBT::setPath(argv[3]);
        getConfig(config, name);
        if (argv[1] == UBT::toLower("--inline"))
            UBT::makeTemplate(std::string(argv[2]), "InlineComponent", name.c_str());
        else if (argv[1] == UBT::toLower("--window"))
            UBT::makeTemplate(std::string(argv[2]), "WindowComponent", name.c_str());
        else if (argv[1] == UBT::toLower("--title-bar"))
            UBT::makeTemplate(std::string(argv[2]), "TitlebarComponent", name.c_str());
        return 0;
    }
}
#endif