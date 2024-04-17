#ifdef UBT_TARGET_FRAMEWORK
#include "ReleaseBuild.hpp"
#include <Generator.hpp>
#include <filesystem>

// Returns the CMake arguments and adds install statements to the "install" std::string&
std::string getInstallStatements(YAML::Node& config, std::string& installs);

void UBT::relBuild(const std::string& name, YAML::Node& config, const std::string& prefix)
{
    UTTE::Generator generator{};
    UTTE::InitialisationResult result;

    result = generator.loadFromFile("../Templates/UntitledImGuiFramework/BuildFiles/BuildDef.hpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << "\x1b[31mThere was an error with the generator when generating the BuildDef.hpp file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    auto& define_or_undefine = generator.pushVariable({ .value = "#define" }, "define_or_undefine");
    auto& define_or_undefine_dev = generator.pushVariable({ .value = "#undef" }, "define_or_undefine_dev");

    std::ofstream out(path + "Generated/BuildDef.hpp");
    out << generator.parse().result->c_str();
    out.close();

    auto currentPath = std::filesystem::path(UBT::getPath().c_str());
    std::filesystem::copy((currentPath/"CMakeLists.txt"), currentPath/"CMakeLists.txt.old");

    std::string installs;

    auto cmakeArgs = getInstallStatements(config, installs);
    // Output temporary CMakeLists.txt
    {
        std::ofstream file;
        file.open(UBT::getPath() + "CMakeLists.txt", std::ios_base::app);
        file << std::endl << installs.c_str(); // Convert to C string because this fucks up on Windows and adds some random data
    }

    std::string systemWide = "--local";
    if (config["system-wide"] && config["system-wide"].as<bool>())
        systemWide = "--system-wide";
#ifdef _WIN32
    auto a = system(("cd " + UBT::getPath() + " && bash export.sh " + name + " " + prefix + " " + systemWide + " " + cmakeArgs).c_str());
#else
    std::string str = "cd " + UBT::getPath() + " && ./export.sh " + name + " " + prefix + " " + systemWide + " " + cmakeArgs;
    std::cout << str << std::endl;
    auto a = system(str.c_str());
#endif
    if (a != 0)
        std::cout << "\x1b[33mThere was an error with running the 'export.sh' script!\x1b[0m";

    result = generator.loadFromFile("../Templates/UntitledImGuiFramework/BuildFiles/BuildDef.hpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << "\x1b[31mThere was an error with the generator when generating the BuildDef.hpp file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    UTTE_VARIABLE_SET_NEW_VAL(define_or_undefine, result, "#undef", UTTE_VARIABLE_TYPE_HINT_NORMAL);
    UTTE_VARIABLE_SET_NEW_VAL(define_or_undefine_dev, result, "#undef", UTTE_VARIABLE_TYPE_HINT_NORMAL);

    out = std::ofstream(path + "Generated/BuildDef.hpp");
    out << generator.parse().result->c_str();
}

enum class InstallPlatform
{
    UNIX,
    WINDOWS,
};

struct CustomInstall
{
    std::string fileDir;
    std::string installDir;
    std::string macroName;
    std::string type;
};

struct InstallDirectories
{
    InstallPlatform platform;
    std::string frameworkDir;
    std::string applicationLibraryDir;
    std::string applicationDir;
    std::string configDir;
    std::string contentDir;

    std::string frameworkIncludeDir;
    std::string applicationIncludeDir;

    std::vector<CustomInstall> customInstalls;
};

void findInstallDirs(YAML::Node& config, InstallDirectories& dirs);
void generateInstallStatements(YAML::Node& config, InstallDirectories& dirs, UTTE::Generator& generator);
void gatherCustomInstalls(YAML::Node& config, InstallDirectories& dirs);
void generateMacroDefinitions(const std::string& name, const std::string& definition, std::string& installs, const std::string& dir);

std::string getInstallStatements(YAML::Node& config, std::string& installs)
{
    auto name = config["name"].as<std::string>();

    InstallDirectories windowsInstallDirectories =
    {
        .platform = InstallPlatform::WINDOWS,
        .frameworkDir = "Program Files/" + name + "/",
        .applicationLibraryDir = "Program Files/" + name + "/",
        .applicationDir = "Program Files/" + name + "/",
        .configDir = "Program Files/" + name + "/",
        .contentDir = "Program Files/" + name + "/",
        .frameworkIncludeDir = "Program Files/" + name + "/",
        .applicationIncludeDir = "Program Files/" + name + "/",
    };

    InstallDirectories unixInstallDirectories =
    {
        .platform = InstallPlatform::UNIX,
        .frameworkDir = "lib/",
        .applicationLibraryDir = "lib/",
        .applicationDir = "bin/",
        .configDir = "etc/" + name + "/",
        .contentDir = "share/config/" + name + "/",
        .frameworkIncludeDir = "include/UntitledImGuiFramework/",
        .applicationIncludeDir = "include/" + name + "/"
    };

    if (config["install-override"])
    {
        auto unixDirs = config["install-override"]["unix"];
        auto windowsDirs = config["install-override"]["windows"];
        if (unixDirs)
            findInstallDirs(unixDirs, unixInstallDirectories);
        if (windowsDirs)
            findInstallDirs(windowsDirs, windowsInstallDirectories);
    }
    gatherCustomInstalls(config, windowsInstallDirectories);
    gatherCustomInstalls(config, unixInstallDirectories);

    installs += "if (WIN32)\n";
    {
        UTTE::Generator generator{};
        auto result = generator.loadFromFile("../Templates/UntitledImGuiFramework/BuildFiles/CMakeInstall.tmpl");
        if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
        {
            std::cout << "\x1b[33mInvalid location for the CMakeInstall template!\x1b[0m" << std::endl;
            std::terminate();
        }
        generateInstallStatements(config, windowsInstallDirectories, generator);
        installs += *generator.parse().result;
    }
    installs += "\nelse()\n";
    {
        UTTE::Generator generator{};
        auto result = generator.loadFromFile("../Templates/UntitledImGuiFramework/BuildFiles/CMakeInstall.tmpl");
        if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
        {
            std::cout << "\x1b[33mInvalid location for the CMakeInstall template!\x1b[0m" << std::endl;
            std::terminate();
        }
        generateInstallStatements(config, unixInstallDirectories, generator);
        installs += *generator.parse().result;
    }

    installs += "\nendif()\n";

    std::string returns;
    if (config["build-mode-static"])
        returns = "-DBUILD_VARIANT_STATIC=" + (config["build-mode-static"].as<bool>() ? std::string("ON") : std::string("OFF"));
    else
        returns = "-DBUILD_VARIANT_STATIC=OFF";
    if (config["build-mode-vendor"])
        returns += " -DBUILD_VARIANT_VENDOR=" + (config["build-mode-vendor"].as<bool>() ? std::string("ON") : std::string("OFF"));
    else
        returns += " -DBUILD_VARIANT_VENDOR=ON";
    if (config["install-framework"])
        returns += " -DUIMGUI_INSTALL_FRAMEWORK=" + (config["install-framework"].as<bool>() ? std::string("ON") : std::string("OFF"));
    else
        returns += " -DUIMGUI_INSTALL_FRAMEWORK=OFF";
    return returns;
}

#define addToDir(x, y) if (config[y]) dirs.x = config[y].as<std::string>()

void findInstallDirs(YAML::Node& config, InstallDirectories& dirs)
{
    addToDir(frameworkDir,              "framework-library-dir");
    addToDir(applicationLibraryDir,     "framework-application-library-dir");
    addToDir(applicationDir,            "application-binary-dir");
    addToDir(configDir,                 "config-dir");
    addToDir(contentDir,                "content-dir");
    addToDir(frameworkIncludeDir,       "framework-include-dir");
    addToDir(applicationIncludeDir,     "application-include-dir");
}

void generateInstallStatements(YAML::Node& config, InstallDirectories& dirs, UTTE::Generator& generator)
{
    auto name = config["name"].as<std::string>();

    std::string applicationDir;
    if (dirs.platform != InstallPlatform::WINDOWS)
    {
        applicationDir += "\n    install(TARGETS " + name + "Lib DESTINATION " + dirs.applicationLibraryDir + ")\n";
        generateMacroDefinitions(name, "UIMGUI_APPLICATION_LIBRARY_DIR", applicationDir, dirs.applicationLibraryDir);
    }

    std::string customDirs;
    for (auto& a : dirs.customInstalls)
    {
        customDirs += "\n    install(" + a.type + " " + a.fileDir + " DESTINATION " + a.installDir + ")\n";
        generateMacroDefinitions(name, a.macroName, customDirs, a.installDir);
    }

    generator.pushVariable({ .value = name                          }, "name"                       );
    generator.pushVariable({ .value = dirs.frameworkDir             }, "framework_dir"              );
    generator.pushVariable({ .value = applicationDir                }, "application_library_dir"    );
    generator.pushVariable({ .value = dirs.applicationDir           }, "application_dir"            );
    generator.pushVariable({ .value = dirs.configDir                }, "config_dir"                 );
    generator.pushVariable({ .value = dirs.contentDir               }, "content_dir"                );
    generator.pushVariable({ .value = dirs.applicationIncludeDir    }, "application_include_dir"    );
    generator.pushVariable({ .value = dirs.frameworkIncludeDir      }, "framework_include_dir"      );
    generator.pushVariable({ .value = customDirs                    }, "custom_installs"            );
}

void gatherCustomInstalls(YAML::Node& config, InstallDirectories& dirs)
{
    if (config["additional-installs"])
    {
        std::string platform;
        switch (dirs.platform)
        {
            case InstallPlatform::WINDOWS:
                platform = "windows";
                break;
            case InstallPlatform::UNIX:
                platform = "unix";
                break;
            default:
                return;
        }

        auto installs = config["additional-installs"][platform];
        if (installs)
        {
            for (const YAML::Node& a : installs)
            {
                if (a["file"] && a["directory"] && a["macro-name"])
                {
                    dirs.customInstalls.push_back(
                    {
                        .fileDir = a["file"].as<std::string>(),
                        .installDir = a["directory"].as<std::string>(),
                        .macroName = a["macro-name"].as<std::string>(),
                        .type = a["is-directory"].as<bool>() ? "DIRECTORY" : "FILES"
                    });
                }
                else
                {
                    std::string filename;
                    if (a["file"])
                        filename = " File in question: " + a["file"].as<std::string>();

                    std::cout << "\x1b[33mWarning: There was a problem with generating a custom install." << filename << "\x1b[0m" << std::endl;
                }
            }
        }
    }
}

void generateMacroDefinitions(const std::string& name, const std::string& definition, std::string& installs, const std::string& dir)
{
    installs += "    target_compile_definitions(UntitledImGuiFramework PRIVATE " + definition +"=\"${CMAKE_INSTALL_PREFIX}/" + dir + "\")\n";
    installs += "    target_compile_definitions(" + name + "Lib PRIVATE " + definition +"=\"${CMAKE_INSTALL_PREFIX}/" + dir + "\")\n";
    installs += "    target_compile_definitions(" + name + " PRIVATE " + definition +"=\"${CMAKE_INSTALL_PREFIX}/" + dir + "\")\n";
}
#endif
