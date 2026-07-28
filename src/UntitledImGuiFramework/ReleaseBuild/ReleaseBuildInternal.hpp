#pragma once
#ifdef UBT_TARGET_FRAMEWORK
#include <Uniform/UniformUtility.h>
#include <Generator.hpp>
#include <exception>

namespace UBT::ReleaseBuildInternal
{
    bool checkBundleCompatibility(ryml::NodeRef config) noexcept;

    std::string generateCMake(const std::filesystem::path& currentPath, ryml::NodeRef config, const std::string& realInstallDir) noexcept;

    void runBuildCommand(ryml::NodeRef config, const std::filesystem::path& currentPath, const std::string& name, const std::string& prefix, const std::string& realInstallDir) noexcept;
}
#endif