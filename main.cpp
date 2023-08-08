#include <UntitledImGuiFramework/Utility.hpp>
#include <UntitledImGuiFramework/CMakeGenerator.hpp>
#include <UntitledImGuiFramework/FileGenerator.hpp>
#include <UntitledImGuiFramework/SourceGenerator.hpp>
#include <UntitledImGuiFramework/ReleaseBuild.hpp>
#include <Generator.hpp>
#include <filesystem>

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
        UTTE::Generator generator;
        auto result = generator.loadFromFile("../Templates/HelpMessage.tmpl", false);
        if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
        {
            std::cout << "\x1b[31mError when opening the HelpMessage.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
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
        else if (argv[1] == UBT::toLower("--build"))
        {
            getConfig(config, name);
            UBT::relBuild(config["name"].as<std::string>(), config, argv[2]);
            return 0;
        }
        return 0;
    }
}