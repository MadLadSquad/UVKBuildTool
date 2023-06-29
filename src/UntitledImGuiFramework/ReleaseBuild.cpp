#include "ReleaseBuild.hpp"
#include <GeneratorCore.hpp>
#include <filesystem>

// Returns the CMake arguments and adds install statements to the "install" std::string&
std::string getInstallStatements(YAML::Node& config, std::string& installs);

void UBT::relBuild(const std::string& name, YAML::Node& config, const std::string& prefix)
{
    {
        UTG::Input in;
        auto result = in.init("../Templates/UntitledImGuiFramework/BuildFiles/BuildDef.hpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
        if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
        {
            std::cout << "\x1b[31mThere was an error with the generator when generating the BuildDef.hpp file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
            std::terminate();
        }
        in["define_or_undefine"] = "#define";
        in["define_or_undefine_dev"] = "#undef";

        std::ofstream out2(path + "Generated/BuildDef.hpp");
        out2 << in.process() << std::endl;
        out2.close();
    }

    auto currentPath = std::filesystem::path(UBT::getPath().c_str());
    std::filesystem::copy((currentPath/"CMakeLists.txt"), currentPath/"CMakeLists.txt.old");

    std::string installs;

    auto cmakeArgs = getInstallStatements(config, installs);
    // Output temporary CMakeLists.txt
    {
        std::ofstream file;
        file.open(UBT::getPath() + "CMakeLists.txt", std::ios_base::app);
        file << std::endl << installs;
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

    UTG::Input in;
    auto result = in.init("../Templates/UntitledImGuiFramework/BuildFiles/BuildDef.hpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mThere was an error with the generator when generating the BuildDef.hpp file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["define_or_undefine"] = "#undef";
    in["define_or_undefine_dev"] = "#undef";

    std::ofstream out2(path + "Generated/BuildDef.hpp");
    out2 << in.process() << std::endl;
    out2.close();
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
void generateInstallStatements(YAML::Node& config, InstallDirectories& dirs, std::string& installs);
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
        .contentDir = "share/" + name + "/",
        .frameworkIncludeDir = "include/" + name + "/",
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
        generateInstallStatements(config, windowsInstallDirectories, installs);
    installs += "\nelse()\n";
        generateInstallStatements(config, unixInstallDirectories, installs);
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
    return returns;
}

#define addToDir(x, y) if (config[y]) dirs.x = config[y].as<std::string>()

void findInstallDirs(YAML::Node& config, InstallDirectories& dirs)
{
    addToDir(frameworkDir, "framework-library-dir");
    addToDir(applicationLibraryDir, "framework-application-library-dir");
    addToDir(applicationDir, "application-binary-dir");
    addToDir(configDir, "config-dir");
    addToDir(contentDir, "content-dir");
    addToDir(frameworkIncludeDir, "framework-include-dir");
    addToDir(applicationIncludeDir, "application-include-dir");
}

void generateInstallStatements(YAML::Node& config, InstallDirectories& dirs, std::string& installs)
{
    auto name = config["name"].as<std::string>();

    installs += "    install(TARGETS UntitledImGuiFramework DESTINATION " + dirs.frameworkDir + ")\n";
    generateMacroDefinitions(name, "UIMGUI_FRAMEWORK_LIBRARY_DIR", installs, dirs.frameworkDir);

    if (dirs.platform != InstallPlatform::WINDOWS)
    {
        installs += "\n    install(TARGETS " + name + "Lib DESTINATION " + dirs.applicationLibraryDir + ")\n";
        generateMacroDefinitions(name, "UIMGUI_APPLICATION_LIBRARY_DIR", installs, dirs.applicationLibraryDir);
    }

    installs += "\n    install(TARGETS " + name + " DESTINATION " + dirs.applicationDir + ")\n";
    generateMacroDefinitions(name, "UIMGUI_APPLICATION_DIR", installs, dirs.applicationDir);

    installs += "\n    install(FILES uvproj.yaml DESTINATION " + dirs.configDir + ")\n";
    generateMacroDefinitions(name, "UIMGUI_PROJECT_DIR", installs, dirs.configDir);

    installs += "\n    install(DIRECTORY \"Config/\" DESTINATION " + dirs.configDir + ")\n";
    generateMacroDefinitions(name, "UIMGUI_CONFIG_DIR", installs, dirs.configDir);

    installs += "\n    install(DIRECTORY \"Content/\" DESTINATION " + dirs.contentDir + ")\n";
    generateMacroDefinitions(name, "UIMGUI_CONTENT_DIR", installs, dirs.contentDir);

    installs += "\n    install(DIRECTORY \"Source/\" DESTINATION " + dirs.applicationIncludeDir + ")\n";
    generateMacroDefinitions(name, "UIMGUI_FRAMEWORK_INCLUDE_DIR", installs, dirs.applicationIncludeDir);

    //installs += "\n    install(DIRECTORY \"Framework/\" DESTINATION " + dirs.frameworkIncludeDir + ")\n";
    generateMacroDefinitions(name, "UIMGUI_APPLICATION_INCLUDE_DIR", installs, dirs.frameworkIncludeDir);

    for (auto& a : dirs.customInstalls)
    {
        installs += "\n    install(" + a.type + " " + a.fileDir + " DESTINATION " + a.installDir + ")\n";
        generateMacroDefinitions(name, a.macroName, installs, a.installDir);
    }
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