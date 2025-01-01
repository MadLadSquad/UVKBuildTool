#ifdef UBT_TARGET_FRAMEWORK
#include "ReleaseBuild.hpp"
#include <Generator.hpp>
#include <filesystem>

// Returns the CMake arguments and adds install statements to the "install" std::string&
std::string getInstallStatements(YAML::Node& config, std::string& installs, const std::string& realInstallDir) noexcept;

void UBT::relBuild(const std::string& name, YAML::Node& config, const std::string& prefix, const std::string& realInstallDir) noexcept
{
    UTTE::Generator generator{};
    UTTE::InitialisationResult result;

    result = generator.loadFromFile(UBT_TEMPLATES_DIR"/BuildFiles/BuildDef.hpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << ERROR << "There was an error with the generator when generating the BuildDef.hpp file! Error code: " << result << END_COLOUR << std::endl;
        std::terminate();
    }
    auto& define_or_undefine = generator.pushVariable({ .value = "#define" }, "define_or_undefine");
    auto& define_or_undefine_dev = generator.pushVariable({ .value = "#undef" }, "define_or_undefine_dev");

    std::ofstream out(path + "Generated/BuildDef.hpp");
    out << generator.parse().result->c_str();
    out.close();

    auto currentPath = std::filesystem::path(getPath().c_str());
    copy((currentPath/"CMakeLists.txt"), currentPath/"CMakeLists.txt.old");

    std::string installs;

    auto cmakeArgs = getInstallStatements(config, installs, realInstallDir);
    // Output temporary CMakeLists.txt
    {
        std::ifstream in(currentPath/"CMakeLists.txt");
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        std::string buffer(size, ' ');
        in.seekg(0);
        in.read(buffer.data(), static_cast<std::streamsize>(size));
        in.close();

        buffer += "\n" + installs;
        for (size_t i = 0; i < buffer.size(); i++)
            if (buffer[i] == '\0' && i < (buffer.size() - 1))
                buffer[i] = ' ';

        std::ofstream file(currentPath/"CMakeLists.txt");
        file << buffer.c_str() << std::endl; // Convert to C string because this fucks up on Windows and adds some random data
        file.close();
    }

    std::string systemWide = "--local";
    if (config["system-wide"] && config["system-wide"].as<bool>())
        systemWide = "--system-wide";
#ifdef _WIN32
    std::string str = "cd " + getPath() + " && bash export.sh " + name + " " + prefix + " " + systemWide + " " + cmakeArgs;
#else
    std::string str = "cd " + getPath() + " && ./export.sh " + name + " " + prefix + " " + systemWide + " " + cmakeArgs;
#endif
    std::cout << str << std::endl;
    if (system(str.c_str()) != 0)
        std::cout << ERROR << "There was an error with running the 'export.sh' script!" << END_COLOUR;

    result = generator.loadFromFile(UBT_TEMPLATES_DIR"/BuildFiles/BuildDef.hpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << ERROR << "There was an error with the generator when generating the BuildDef.hpp file! Error code: " << result << END_COLOUR << std::endl;
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
    MACOS,
    WASM
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

void findInstallDirs(YAML::Node& config, InstallDirectories& dirs) noexcept;
void generateInstallStatements(YAML::Node& config, const InstallDirectories& dirs, UTTE::Generator& generator) noexcept;
void gatherCustomInstalls(YAML::Node& config, InstallDirectories& dirs) noexcept;
void generateMacroDefinitions(const std::string& name, const std::string& definition, std::string& installs, const std::string& dir, InstallPlatform platform) noexcept;

void addParsedPlatformConfigToCMakeLists(YAML::Node& config, const InstallDirectories& installDirs, std::string& installs) noexcept
{
    UTTE::Generator generator{};
    const auto result = generator.loadFromFile(UBT_TEMPLATES_DIR"/BuildFiles/CMakeInstall.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << ERROR << "Invalid location for the CMakeInstall template!\x1b[0m" << std::endl;
        std::terminate();
    }
    generateInstallStatements(config, installDirs, generator);
    installs += *generator.parse().result;
}

std::string getInstallStatements(YAML::Node& config, std::string& installs, const std::string& realInstallDir) noexcept
{
    auto name = config["name"].as<std::string>();

    InstallDirectories windowsInstallDirectories =
    {
        .platform = InstallPlatform::WINDOWS,
        .frameworkDir =                 "Program Files/" + name + "/",
        .applicationLibraryDir =        "Program Files/" + name + "/",
        .applicationDir =               "Program Files/" + name + "/",
        .configDir =                    "Program Files/" + name + "/",
        .contentDir =                   "Program Files/" + name + "/",
        .frameworkIncludeDir =          "Program Files/" + name + "/",
        .applicationIncludeDir =        "Program Files/" + name + "/",
    };

    InstallDirectories unixInstallDirectories =
    {
        .platform = InstallPlatform::UNIX,
        .frameworkDir =                 "lib64/",
        .applicationLibraryDir =        "lib64/",
        .applicationDir =               "bin/",
        .configDir =                    "etc/" + name + "/",
        .contentDir =                   "share/config/" + name + "/",
        .frameworkIncludeDir =          "include/UntitledImGuiFramework/",
        .applicationIncludeDir =        "include/" + name + "/"
    };

    InstallDirectories macosInstallDirectories =
    {
        .platform = InstallPlatform::MACOS,
        .frameworkDir =                 "lib64/",
        .applicationLibraryDir =        "lib64/",
        .applicationDir =               "bin/",
        .configDir =                    "etc/" + name + "/",
        .contentDir =                   "share/config/" + name + "/",
        .frameworkIncludeDir =          "include/UntitledImGuiFramework/",
        .applicationIncludeDir =        "include/" + name + "/"
    };

    // TODO: Configure WASM traget installs
    InstallDirectories wasmInstallDirectories =
    {
        .platform = InstallPlatform::WASM,
        .frameworkDir =                 "lib64/",
        .applicationLibraryDir =        "lib64/",
        .applicationDir =               "bin/",
        .configDir =                    "etc/" + name + "/",
        .contentDir =                   "share/config/" + name + "/",
        .frameworkIncludeDir =          "include/UntitledImGuiFramework/",
        .applicationIncludeDir =        "include/" + name + "/"
    };

    if (config["install-override"])
    {
        auto unixDirs = config["install-override"]["unix"];
        auto windowsDirs = config["install-override"]["windows"];
        auto macosDirs = config["install-override"]["macos"];
        auto wasmDirs = config["install-override"]["wasm"];
        if (unixDirs)
            findInstallDirs(unixDirs, unixInstallDirectories);
        if (windowsDirs)
            findInstallDirs(windowsDirs, windowsInstallDirectories);
        if (macosDirs)
            findInstallDirs(macosDirs, macosInstallDirectories);
        if (wasmDirs)
            findInstallDirs(wasmDirs, wasmInstallDirectories);
    }
    gatherCustomInstalls(config, windowsInstallDirectories);
    gatherCustomInstalls(config, unixInstallDirectories);
    gatherCustomInstalls(config, macosInstallDirectories);
    gatherCustomInstalls(config, wasmInstallDirectories);

    installs += "if (EMSCRIPTEN)\n";
        addParsedPlatformConfigToCMakeLists(config, wasmInstallDirectories, installs);
    installs += "\nelseif (APPLE)\n";
        addParsedPlatformConfigToCMakeLists(config, macosInstallDirectories, installs);
    installs += "\nelseif (WIN32)\n";
        addParsedPlatformConfigToCMakeLists(config, windowsInstallDirectories, installs);
    installs += "\nelse()\n";
       addParsedPlatformConfigToCMakeLists(config, unixInstallDirectories, installs);
    installs += "\nendif()\n";

    std::string returns = "-DUIMGUI_INSTALL_PREFIX=" + realInstallDir;
    if (config["build-mode-static"])
        returns += " -DBUILD_VARIANT_STATIC=" + (config["build-mode-static"].as<bool>() ? std::string("ON") : std::string("OFF"));
    else
        returns += " -DBUILD_VARIANT_STATIC=OFF";
    if (config["build-mode-vendor"])
        returns += " -DBUILD_VARIANT_VENDOR=" + (config["build-mode-vendor"].as<bool>() ? std::string("ON") : std::string("OFF"));
    else
        returns += " -DBUILD_VARIANT_VENDOR=ON";

    if (config["install-framework"])
    {
        bool bInstallFramework = config["install-framework"].as<bool>();
        returns += " -DUIMGUI_INSTALL_FRAMEWORK=" + (bInstallFramework ? std::string("ON") : std::string("OFF"));
        returns += " -DUIMGUI_USE_PKGCONF_FOR_INSTALL=" + (bInstallFramework ? std::string("OFF") : std::string("ON"));
    }
    else
        returns += " -DUIMGUI_INSTALL_FRAMEWORK=OFF";
    return returns;
}

#define addToDir(x, y) if (config[y]) dirs.x = config[y].as<std::string>()

void findInstallDirs(YAML::Node& config, InstallDirectories& dirs) noexcept
{
    addToDir(frameworkDir,              "framework-library-dir");
    addToDir(applicationLibraryDir,     "framework-application-library-dir");
    addToDir(applicationDir,            "application-binary-dir");
    addToDir(configDir,                 "config-dir");
    addToDir(contentDir,                "content-dir");
    addToDir(frameworkIncludeDir,       "framework-include-dir");
    addToDir(applicationIncludeDir,     "application-include-dir");
}

void generateInstallStatements(YAML::Node& config, const InstallDirectories& dirs, UTTE::Generator& generator) noexcept
{
    const auto name = config["name"].as<std::string>();

    std::string applicationDir;
    if (dirs.platform != InstallPlatform::WINDOWS)
    {
        applicationDir += "\n    install(TARGETS " + name + "Lib DESTINATION \"" + dirs.applicationLibraryDir + "\")\n";
        generateMacroDefinitions(name, "UIMGUI_APPLICATION_LIBRARY_DIR", applicationDir, dirs.applicationLibraryDir, dirs.platform);
    }

    std::string customDirs;
    for (const auto& a : dirs.customInstalls)
    {
        customDirs += "\n    install(" + a.type + " " + a.fileDir + " DESTINATION \"" + a.installDir + "\")\n";
        generateMacroDefinitions(name, a.macroName, customDirs, a.installDir, dirs.platform);
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

void gatherCustomInstalls(YAML::Node& config, InstallDirectories& dirs) noexcept
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
            case InstallPlatform::MACOS:
                platform = "macos";
                break;
            case InstallPlatform::WASM:
                platform = "wasm";
                break;
            default:
                return;
        }

        const auto installs = config["additional-installs"][platform];
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

                    std::cout << WARNING << "Warning: There was a problem with generating a custom install." << filename << END_COLOUR << std::endl;
                }
            }
        }
    }
}

void generateMacroDefinitions(const std::string& name, const std::string& definition, std::string& installs, const std::string& dir, const InstallPlatform platform) noexcept
{
    installs += "    multicast(target_compile_definitions PRIVATE " + definition +"=\"${UIMGUI_INSTALL_PREFIX}/" + dir + "\")\n";
}
#endif
