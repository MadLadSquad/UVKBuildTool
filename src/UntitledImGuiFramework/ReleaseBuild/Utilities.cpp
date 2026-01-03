#include "ReleaseBuildInternal.hpp"
#include <format>
#ifdef UBT_TARGET_FRAMEWORK

bool UBT::ReleaseBuildInternal::checkBundleCompatibility(YAML::Node& config) noexcept
{
#ifdef __APPLE__
    if (
        config["build-mode-vendor"]                 &&
        !config["build-mode-vendor"].as<bool>()     &&
        config["macos"]                             &&
        config["macos"]["bundle"]                   &&
        config["macos"]["bundle"].as<bool>())
    {
        std::cout << ERROR << "You're currently trying to build an application as a macOS application bundle, but with system libraries, which is not possible!\n"
                                "Application bundles are made to be self-contained, which means that you are forced to vendor all your dependencies, except for OS libraries!" << END_COLOUR << std::endl;
        return false;
    }
#endif
    return true;
}

void UBT::ReleaseBuildInternal::runBuildCommand(YAML::Node& config, const std::filesystem::path& currentPath, const std::string& name, const std::string& prefix, const std::string& realInstallDir) noexcept
{
    std::string systemWide = "--local";
    if (config["system-wide"] && config["system-wide"].as<bool>())
        systemWide = "--system-wide";

    const std::string command =     std::format(
                                        "cd {} && {} " UBT_FRAMEWORK_DIR "/export.sh {} {} {} {}",
                                        currentPath.string(),
#ifdef _WIN32
                                        "bash",
#else
                                        "",
#endif
                                        name,
                                        prefix,
                                        systemWide,
                                        generateCMake(currentPath, config, realInstallDir)
                                    );

    std::cout << command << std::endl;
    if (system(command.c_str()) != 0)
        std::cout << ERROR << "There was an error with running the 'export.sh' script!" << END_COLOUR;
}
#endif
