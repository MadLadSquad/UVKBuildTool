#include "ReleaseBuildInternal.hpp"
#include <format>
#ifdef UBT_TARGET_FRAMEWORK

enum class InstallPlatform
{
    UNIX,
    WINDOWS,
    MACOS
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

static void generateMacroDefinitions(const std::string& definition, std::string& installs, const std::string& dir) noexcept
{
    installs += std::format("    multicast(target_compile_definitions PRIVATE {}=\"${{UIMGUI_INSTALL_PREFIX}}/{}\")\n", definition, dir);
}

static void findInstallDirs(YAML::Node& config, InstallDirectories& dirs) noexcept
{
#define addToDir(x, y) if (config[y]) dirs.x = config[y].as<std::string>()

    addToDir(frameworkDir,              "framework-library-dir");
    addToDir(applicationLibraryDir,     "framework-application-library-dir");
    addToDir(applicationDir,            "application-binary-dir");
    addToDir(configDir,                 "config-dir");
    addToDir(contentDir,                "content-dir");
    addToDir(frameworkIncludeDir,       "framework-include-dir");
    addToDir(applicationIncludeDir,     "application-include-dir");
}

static void generateInstallStatements(YAML::Node& config, const InstallDirectories& dirs, UTTE::Generator& generator) noexcept
{
    const auto name = config["name"].as<std::string>();

    std::string applicationDir;
    if (dirs.platform != InstallPlatform::WINDOWS)
    {
        applicationDir += std::format("\n    install(TARGETS {}Lib DESTINATION \"{}\")\n", name, dirs.applicationLibraryDir);
        if (dirs.platform == InstallPlatform::MACOS && config["macos"] && config["macos"]["bundle"] && config["macos"]["bundle"].as<bool>())
        {
            applicationDir += std::format(R"(
    install(FILES "Framework/ThirdParty/vulkan/libvulkan.1.dylib" DESTINATION "{}" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    install(FILES ${{UIMGUI_THIRD_PARTY_LIBS}} DESTINATION "{}" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    install(FILES "${{PROJECT_BINARY_DIR}}/Info.plist" DESTINATION "{}/../")
    install(FILES "Config/Resources/Icon.icns" DESTINATION "{}/../")
)", dirs.applicationLibraryDir, dirs.applicationLibraryDir, dirs.applicationLibraryDir, dirs.contentDir);
        }
        generateMacroDefinitions("UIMGUI_APPLICATION_LIBRARY_DIR", applicationDir, dirs.applicationLibraryDir);
    }

    std::string customDirs;
    for (const auto& a : dirs.customInstalls)
    {
        customDirs += std::format("\n    install({} {} DESTINATION \"{}\")\n", a.type, a.fileDir, a.installDir);
        generateMacroDefinitions(a.macroName, customDirs, a.installDir);
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

static void addParsedPlatformConfigToCMakeLists(YAML::Node& config, const InstallDirectories& installDirs, std::string& installs) noexcept
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

static void gatherCustomInstalls(YAML::Node& config, InstallDirectories& dirs) noexcept
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

// Returns the CMake arguments and adds install statements to the "install" std::string&
static std::string getInstallStatements(YAML::Node& config, std::string& installs, const std::string& realInstallDir) noexcept
{
    auto name = config["name"].as<std::string>();

    // macOS settings. Bundle for whether to ship as an application bundle
    bool bBundle = false;
    if (config["macos"] && config["macos"]["bundle"])
        bBundle = config["macos"]["bundle"].as<bool>();

    std::string unixConfigDir = std::format("share/config/{}/Config/", name);
    std::string unixContentDir = std::format("share/config/{}/Content/", name);
    if (realInstallDir == "/")
    {
        unixConfigDir = std::format("etc/{}/Config/", name);
        unixContentDir = std::format("etc/{}/Content/", name);
    }
    else if (realInstallDir.ends_with(".local/") || realInstallDir.ends_with(".local"))
        unixConfigDir = std::format("../.config/{}/Config/", name);

    InstallDirectories windowsInstallDirectories =
    {
        .platform = InstallPlatform::WINDOWS,
        .frameworkDir =                 "Program Files/" + name + "/lib/",
        .applicationLibraryDir =        "Program Files/" + name + "/lib/",
        .applicationDir =               "Program Files/" + name + "/bin/",
        .configDir =                    "Program Files/" + name + "/Config/",
        .contentDir =                   "Program Files/" + name + "/Content/",
        .frameworkIncludeDir =          "Program Files/" + name + "/include/UntitledImGuiFramework/",
        .applicationIncludeDir =        "Program Files/" + name + "/include/" + name + "/",
    };

    InstallDirectories unixInstallDirectories =
    {
        .platform = InstallPlatform::UNIX,
        .frameworkDir =                 "lib64/",
        .applicationLibraryDir =        "lib64/",
        .applicationDir =               "bin/",
        .configDir =                    unixConfigDir,
        .contentDir =                   unixContentDir,
        .frameworkIncludeDir =          "include/UntitledImGuiFramework/",
        .applicationIncludeDir =        "include/" + name + "/"
    };

    InstallDirectories macosInstallDirectories =
    {
        .platform = InstallPlatform::MACOS,
        .frameworkDir =                 bBundle ? name + ".app/Contents/Frameworks/"                                 : "lib64/",
        .applicationLibraryDir =        bBundle ? name + ".app/Contents/Frameworks/"                                 : "lib64/",
        .applicationDir =               bBundle ? name + ".app/Contents/MacOS"                                       : "bin/",
        .configDir =                    bBundle ? name + ".app/Contents/Resources/Config/"                           : unixConfigDir,
        .contentDir =                   bBundle ? name + ".app/Contents/Resources/Content/"                          : unixContentDir,
        .frameworkIncludeDir =          bBundle ? name + ".app/Contents/Frameworks/include/UntitledImGuiFramework"   : "include/UntitledImGuiFramework/",
        .applicationIncludeDir =        bBundle ? name + ".app/Contents/Frameworks/include/" + name + "/"               : "include/" + name + "/"
    };

    if (config["install-override"])
    {
        auto unixDirs = config["install-override"]["unix"];
        auto windowsDirs = config["install-override"]["windows"];
        auto macosDirs = config["install-override"]["macos"];
        if (unixDirs)
            findInstallDirs(unixDirs, unixInstallDirectories);
        if (windowsDirs)
            findInstallDirs(windowsDirs, windowsInstallDirectories);
        if (macosDirs)
            findInstallDirs(macosDirs, macosInstallDirectories);
    }
    gatherCustomInstalls(config, windowsInstallDirectories);
    gatherCustomInstalls(config, unixInstallDirectories);
    gatherCustomInstalls(config, macosInstallDirectories);

    installs += "if (APPLE)\n";
        addParsedPlatformConfigToCMakeLists(config, macosInstallDirectories, installs);
    installs += "\nelseif (WIN32)\n";
        addParsedPlatformConfigToCMakeLists(config, windowsInstallDirectories, installs);
    installs += "\nelse()\n";
       addParsedPlatformConfigToCMakeLists(config, unixInstallDirectories, installs);
    installs += "\nendif()\n";

    std::string buildStatic = "OFF";
    if (config["build-mode-static"] && config["build-mode-static"].as<bool>())
        buildStatic = "ON";

    std::string buildVendor = "ON";
    if (config["build-mode-vendor"])
    {
        if (config["build-mode-vendor"].as<bool>())
            buildVendor = "ON";
        else
            buildVendor = "OFF";
    }

    std::string installFramework = "OFF";
    std::string srcPrefix{};

    // Since we're building for a bundle it's important to also install the framework
    if (bBundle)
    {
        installFramework = "ON";
        srcPrefix = "-DUIMGUI_IN_BUNDLE=ON";
    }
    else if (config["install-framework"])
    {
        bool bShouldInstall = config["install-framework"].as<bool>();
        installFramework = bShouldInstall ? "ON" : "OFF";

        if (!bShouldInstall)
            srcPrefix = "-DUIMGUI_SRC_PREFIX=\"" UBT_FRAMEWORK_DIR "\"";
    }


    std::string returns =   std::format(
                                        "-DUIMGUI_INSTALL_PREFIX={} -DBUILD_VARIANT_STATIC={} {} -DBUILD_VARIANT_VENDOR={} -DUIMGUI_INSTALL_FRAMEWORK={} {}",
                                        realInstallDir,
                                        buildStatic,
#ifdef UBT_DO_NOT_BUILD_FRAMEWORK
                                        " -DUIMGUI_SKIP_FRAMEWORK=ON",
#else
                                        "",
#endif
                                        buildVendor,
                                        installFramework,
                                        srcPrefix
                            );
    return returns;
}

std::string UBT::ReleaseBuildInternal::generateCMake(const std::filesystem::path& currentPath, YAML::Node& config, const std::string& realInstallDir) noexcept
{
    copy((currentPath/"CMakeLists.txt"), currentPath/"CMakeLists.txt.old");

    std::string installs;

    auto cmakeArgs = getInstallStatements(config, installs, realInstallDir);
    // Output temporary CMakeLists.txt
    {
        auto buffer = UBT::loadFileToString((currentPath/"CMakeLists.txt").string());
        buffer += "\n" + installs;

        // Sanitize because this breaks for some goddamn reason!?!?!?!?!?!?
        for (size_t i = 0; i < buffer.size(); i++)
            if (buffer[i] == '\0' && i < (buffer.size() - 1))
                buffer[i] = ' ';

        std::ofstream file(currentPath/"CMakeLists.txt");
        file << buffer.c_str() << std::endl; // Convert to C string because this fucks up on Windows and adds some random data
        file.close();
    }
    return cmakeArgs;
}
#endif
