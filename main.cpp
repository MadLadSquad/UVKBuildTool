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

/**
 * @brief Check if an arguments is withing bounds and that the args array is not null
 * @arg x - boolean expression to use to check
 * @arg y - Error message
 */
#define CHECK_BOUNDS(x, y) if ((x) || args == nullptr) {      \
    std::cout << ERROR << (y) << END_COLOUR << std::endl;     \
    exit(UBT::showHelp(true));                                \
}

/**
 * @brief Sets the UVKBuildTool project path and gets config
 * @arg x - Project path as string
 */
#define SETUP_WORKDIR(x) UBT::setPath(x); getConfig(config, name);

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

                    CHECK_BOUNDS(size == 0, "Invalid argument, generate requires a path to a UVKBuildTool project!");
                    SETUP_WORKDIR(args[0]);

                    std::string startupLevelName;
                    if (config["name"])
                        name = config["name"].as<std::string>();

                    auto path = std::filesystem::path(UBT::getPath());
                    if (!std::filesystem::exists(path/"Generated"))
                        std::filesystem::create_directory(path/"Generated");
                    if (!std::filesystem::exists(path/"Framework"))
                        std::filesystem::create_directory_symlink(std::filesystem::current_path().parent_path().parent_path()/"Framework", path/"Framework");
                    if (!std::filesystem::exists(path/"UVKBuildTool"))
                        std::filesystem::create_directory_symlink(std::filesystem::current_path().parent_path(), path/"UVKBuildTool");
                    if (!std::filesystem::exists(path/"export.sh"))
                        std::filesystem::copy_file(std::filesystem::current_path().parent_path().parent_path()/"export.sh", path/"export.sh");

                    UBT::generateMain(name.c_str());
                    UBT::generateDef();

                    std::filesystem::copy_file(std::filesystem::path("../Templates/UntitledImGuiFramework/Sources/Config.hpp.tmpl"),
                                               std::filesystem::path(UBT::getPath())/"Generated/Config.hpp", std::filesystem::copy_options::overwrite_existing);
                    exit(0);
                },
            },
            UCLI::Parser::ArrayFlag{
                .longType = "install",
                .shortType = "i",
                .func = [](UCLI::Parser::ArrayFlag*, char** args, size_t size) -> void {
                    YAML::Node config;
                    std::string name;

                    CHECK_BOUNDS(size == 0, "Invalid argument, generate requires a path to a UVKBuildTool project!");
                    SETUP_WORKDIR(args[0]);

                    std::string startupLevelName;
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

                    CHECK_BOUNDS(size < 2, "Invalid argument, inline requires a class name path to a UVKBuildTool project!");
                    SETUP_WORKDIR(args[1]);

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

                    CHECK_BOUNDS(size < 2, "Invalid argument, window requires a class name path to a UVKBuildTool project!");
                    SETUP_WORKDIR(args[1]);

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

                    CHECK_BOUNDS(size < 2, "Invalid argument, title-bar requires a class name path to a UVKBuildTool project!");
                    SETUP_WORKDIR(args[1]);

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

                    CHECK_BOUNDS(size < 3, "Invalid argument, build requires a staging path, installation path and a path to a UVKBuildTool project!");
                    SETUP_WORKDIR(args[2]);

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
                CHECK_BOUNDS(size < 2, "Invalid argument, build requires an export path and a UVKBuildTool project path!");
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
