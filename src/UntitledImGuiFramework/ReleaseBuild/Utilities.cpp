#include "ReleaseBuildInternal.hpp"
#include <format>
#ifdef UBT_TARGET_FRAMEWORK

bool UBT::ReleaseBuildInternal::checkBundleCompatibility(ryml::NodeRef config) noexcept
{
#ifdef __APPLE__
    auto buildModeVendor = config["build-mode-vendor"];
    auto macos = config["macos"];

    if (ryml::keyValid(buildModeVendor) && ryml::keyValid(macos))
    {
        auto bundle = macos["bundle"];
        if (ryml::keyValid(bundle))
        {
            bool vendor{};
            buildModeVendor >> vendor;

            bool mac{};
            bundle >> mac;

            if (!vendor && mac)
            {
                std::cout << ERROR << "You're currently trying to build an application as a macOS application bundle, but with system libraries, which is not possible!\n"
                                "Application bundles are made to be self-contained, which means that you are forced to vendor all your dependencies, except for OS libraries!" << END_COLOUR << std::endl;
                return false;
            }
        }
    }
#endif
    return true;
}

void UBT::ReleaseBuildInternal::runBuildCommand(ryml::NodeRef config, const std::filesystem::path& currentPath, const std::string& name, const std::string& prefix, const std::string& realInstallDir) noexcept
{
    std::string systemWide = "--local";
    auto sw = config["system-wide"];
    if (ryml::keyValid(sw))
    {
        bool val{};
        sw >> val;
        if (val)
            systemWide = "--system-wide";
    }

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
