#ifdef UBT_TARGET_FRAMEWORK
    #include <UntitledImGuiFramework/Utility.hpp>
    #include <UntitledImGuiFramework/CMakeGenerator.hpp>
    #include <UntitledImGuiFramework/FileGenerator.hpp>
    #include <UntitledImGuiFramework/SourceGenerator.hpp>
    #include <UntitledImGuiFramework/ReleaseBuild.hpp>
#elif UBT_TARGET_WEB
    #include <Web/ConfigManager.hpp>
    #include <Web/Functions.hpp>
#endif
#include <Generator.hpp>
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "ucli/CLIParser.hpp"

#define ERROR "\x1b[31m"
#define WARNING "\x1b[33m"
#define SUCCESS "\x1b[32m"
#define END_COLOUR "\x1b[0m"

void getConfig(YAML::Node& config, std::string& name)
{
    try
    {
        std::string tmp = UBT::getPath() + "uvproj.yaml";
        config = YAML::LoadFile(tmp);
    }
    catch (YAML::BadFile&)
    {
        std::cout << ERROR << "Could not locate file" << END_COLOUR << std::endl;
        std::terminate();
    }

    if (config["name"])
        name = config["name"].as<std::string>();
}

#ifdef UBT_TARGET_FRAMEWORK
int main(int argc, char** argv)
{
    UBT::setPath("../../");
    //UCLI::Parser parser{};
    //parser.setUnknownArgumentCallback([](const char* name, void* data) -> void {
    //    std::cout << ERROR << "Unknown argument error: " << name << std::endl;
    //    // TODO: Add help function here
    //}, nullptr);
    //parser.parse(argc, argv, )

    if (argc < 2)
    {
        std::cout << ERROR << "Not enough arguments passed into the generator!" << END_COLOUR << std::endl;
        return 0;
    }
    if (argv[1] == UBT::toLower("--help"))
    {
        UTTE::Generator generator;
        auto result = generator.loadFromFile("../Templates/HelpMessage.tmpl");
        if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
        {
            std::cout << ERROR << "Error when opening the HelpMessage.tmpl file! Error code: " << static_cast<int>(result) << END_COLOUR << std::endl;
            std::terminate();
        }
        std::string buf;
        std::string tmp;

        std::ifstream instr("../Templates/UntitledImGuiFramework/ArgumentsRegistry.tmpl");
        while (std::getline(instr, tmp))
            buf += tmp + "\n";

        generator.pushVariable({ .value = buf }, "arguments");
        std::cout << *generator.parse().result << std::endl;
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
        if (config["name"])
            name = config["name"].as<std::string>();

        UBT::generateMain(name.c_str());
        UBT::generateDef();
        std::filesystem::copy_file(std::filesystem::path("../Templates/UntitledImGuiFramework/Sources/Config.hpp.tmpl"),
                                   std::filesystem::path(UBT::getPath())/"Generated/Config.hpp");
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

        std::filesystem::copy_file(std::filesystem::path("../Templates/UntitledImGuiFramework/Sources/Config.hpp.tmpl"),
                                   std::filesystem::path(UBT::getPath())/"Generated/Config.hpp");

        return 0;
    }

    if (argc < 4)
    {
        std::cout << ERROR << "Error: Not enough arguments passed into the generator!" << END_COLOUR << std::endl;
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

    if (argc < 5)
    {
        std::cout << ERROR << "Error: Not enough arguments passed into the generator!" << END_COLOUR << std::endl;
        return 0;
    }
    else if (argc == 5)
    {
        UBT::setPath(argv[4]);
        getConfig(config, name);
        if (argv[1] == UBT::toLower("--build"));
            UBT::relBuild(config["name"].as<std::string>(), config, argv[2], argv[3]);
    }
    return 0;
}
#elif UBT_TARGET_WEB
int main(int argc, char** argv)
{
    UBT::setPath("../../");
    if (argc < 2)
        return UBT::showHelp(false);
    if (argv[1] == UBT::toLower("--help"))
        return UBT::showHelp(false);

    if (argc < 4)
    {
        std::cout << ERROR << "Not enough arguments passed into the generator!" << END_COLOUR << std::endl;
        return UBT::showHelp(true);
    }
    else if (argv[1] == UBT::toLower("--build"))
        UBT::buildMain(argv[2], argv[3]);
    return 0;
}
#endif
