#pragma once
#ifdef UBT_TARGET_FRAMEWORK
#include <Uniform/UniformUtility.h>
#include <Generator.hpp>
#include <exception>

namespace UBT::ReleaseBuildInternal
{
    bool checkBundleCompatibility(ryml::NodeRef config) noexcept;

    // Puts the project's CMakeLists.txt back from CMakeLists.txt.old and removes the backup. A no-op when
    // there is no backup. "bRecovering" only selects the wording of the message for the interrupted-build case
    void restoreCMake(const std::filesystem::path& currentPath, bool bRecovering = false) noexcept;

    std::string generateCMake(const std::filesystem::path& currentPath, ryml::NodeRef config, const std::string& realInstallDir) noexcept;

    void runBuildCommand(ryml::NodeRef config, const std::filesystem::path& currentPath, const std::string& name, const std::string& prefix, const std::string& realInstallDir) noexcept;
}
#endif