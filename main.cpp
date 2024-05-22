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
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "ucli/CLIParser.hpp"

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
    UCLI::Parser parser{};
    parser.setUnknownArgumentCallback([](const char* name, void* data) -> void {
        std::cout << ERROR << "Unknown argument error: " << name << END_COLOUR << std::endl;
        exit(UBT::showHelp(false));
    }, nullptr);
    parser.parse(argc, argv, {
            UCLI::Parser::ArrayFlag{
                .longType = "generate",
                .shortType = "g",
                .func = [](UCLI::Parser::ArrayFlag*, char** args, size_t size) -> void {
                    YAML::Node config;
                    std::string name;

                    if (size == 0 || args == nullptr)
                    {
                        std::cout << ERROR << "Invalid argument, generate requires a path to a UVKBuildTool project!" << END_COLOUR << std::endl;
                        exit(UBT::showHelp(true));
                    }
                    UBT::setPath(args[0]);
                    getConfig(config, name);

                    std::string startupLevelName;
                    std::ifstream i(UBT::getPath() + "Generated/ActorList.hpp");
                    if (config["name"])
                        name = config["name"].as<std::string>();

                    UBT::generateMain(name.c_str());
                    UBT::generateDef();
                    std::filesystem::copy_file(std::filesystem::path("../Templates/UntitledImGuiFramework/Sources/Config.hpp.tmpl"),
                                               std::filesystem::path(UBT::getPath())/"Generated/Config.hpp");
                    exit(0);
                },
            },
            UCLI::Parser::ArrayFlag{
                .longType = "install",
                .shortType = "i",
                .func = [](UCLI::Parser::ArrayFlag*, char** args, size_t size) -> void {
                    YAML::Node config;
                    std::string name;

                    if (size == 0 || args == nullptr)
                    {
                        std::cout << ERROR << "Invalid argument, install requires a path to a UVKBuildTool project!" << END_COLOUR << std::endl;
                        exit(UBT::showHelp(true));
                    }
                    UBT::setPath(args[0]);
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
                    exit(0);
                },
            },
            UCLI::Parser::ArrayFlag{
                .longType = "inline",
                .shortType = "",
                .func = [](UCLI::Parser::ArrayFlag*, char** args, size_t size) -> void {
                    YAML::Node config;
                    std::string name;

                    if (size < 2 || args == nullptr)
                    {
                        std::cout << ERROR << "Invalid argument, inline requires a class name path to a UVKBuildTool project!" << END_COLOUR << std::endl;
                        exit(UBT::showHelp(true));
                    }
                    UBT::setPath(args[1]);
                    getConfig(config, name);

                    UBT::makeTemplate(std::string(args[0]), "InlineComponent", name.c_str());
                    exit(0);
                },
            },
            UCLI::Parser::ArrayFlag{
                .longType = "window",
                .shortType = "",
                .func = [](UCLI::Parser::ArrayFlag*, char** args, size_t size) -> void {
                    YAML::Node config;
                    std::string name;

                    if (size < 2 || args == nullptr)
                    {
                        std::cout << ERROR << "Invalid argument, window requires a class name and path to a UVKBuildTool project!" << END_COLOUR << std::endl;
                        exit(UBT::showHelp(true));
                    }
                    UBT::setPath(args[1]);
                    getConfig(config, name);

                    UBT::makeTemplate(std::string(args[0]), "WindowComponent", name.c_str());
                    exit(0);
                },
            },
            UCLI::Parser::ArrayFlag{
                .longType = "title-bar",
                .shortType = "",
                .func = [](UCLI::Parser::ArrayFlag*, char** args, size_t size) -> void {
                    YAML::Node config;
                    std::string name;

                    if (size < 2 || args == nullptr)
                    {
                        std::cout << ERROR << "Invalid argument, title-bar requires a class name and path to a UVKBuildTool project!" << END_COLOUR << std::endl;
                        exit(UBT::showHelp(true));
                    }
                    UBT::setPath(args[1]);
                    getConfig(config, name);

                    UBT::makeTemplate(std::string(args[0]), "TitlebarComponent", name.c_str());
                    exit(0);
                },
            },
            UCLI::Parser::ArrayFlag{
                .longType = "build",
                .shortType = "b",
                .func = [](UCLI::Parser::ArrayFlag*, char** args, size_t size) -> void {
                    YAML::Node config;
                    std::string name;

                    if (size < 3 || args == nullptr)
                    {
                        std::cout << ERROR << "Invalid argument, build requires a staging path, installation path and a path to a UVKBuildTool project!" << END_COLOUR << std::endl;
                        exit(UBT::showHelp(true));
                    }
                    UBT::setPath(args[2]);
                    getConfig(config, name);

                    UBT::relBuild(config["name"].as<std::string>(), config, args[0], args[1]);
                    exit(0);
                },
            },
        }, {}, {
            UCLI::Parser::BooleanFlagWithFunc{
                .longType = "help",
                .shortType = "h",
                .func = [](UCLI::Parser::BooleanFlagWithFunc*) -> void {
                    exit(UBT::showHelp(false));
                },
            },
        }
    );
    return 0;
}
#elif UBT_TARGET_WEB
int main(int argc, char** argv)
{
    UBT::setPath("../../");
    UCLI::Parser parser{};
    parser.setUnknownArgumentCallback([](const char* name, void* data) -> void {
        std::cout << ERROR << "Unknown argument error: " << name << END_COLOUR << std::endl;
        exit(UBT::showHelp(false));
    }, nullptr);
    parser.parse(argc, argv, {
        UCLI::Parser::ArrayFlag {
            .longType = "build",
            .shortType = "b",
            .func = [](UCLI::Parser::ArrayFlag*, char** args, size_t size) -> void {
                if (size < 2)
                {
                    std::cout << ERROR << "Invalid argument, build requires an export path and a UVKBuildTool project path!" << END_COLOUR << std::endl;
                    exit(UBT::showHelp(true));
                }
                UBT::buildMain(args[0], args[1]);
                exit(0);
            }
        },
    }, {}, {
        UCLI::Parser::BooleanFlagWithFunc {
            .longType = "help",
            .shortType = "h",
            .func = [](UCLI::Parser::BooleanFlagWithFunc*) -> void {
                exit(UBT::showHelp(false));
            }
        }
    });
    return 0;
}
#endif
