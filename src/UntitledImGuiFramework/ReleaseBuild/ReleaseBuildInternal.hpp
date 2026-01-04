#pragma once
#ifdef UBT_TARGET_FRAMEWORK
#include "../Utility.hpp"
#include <Generator.hpp>
#include <exception>

namespace UBT::ReleaseBuildInternal
{
    bool checkBundleCompatibility(ryml::NodeRef config) noexcept;

    void generateTemporaryBuildDef(const std::filesystem::path& currentPath, UTTE::Generator& generator, UTTE::InitialisationResult& result, UTTE::Function** define_or_undefine, UTTE::Function** define_or_undefine_dev) noexcept;
    void restoreBuildDef(const std::filesystem::path& currentPath, UTTE::Generator& generator, UTTE::InitialisationResult& result, UTTE::Function* define_or_undefine, UTTE::Function* define_or_undefine_dev) noexcept;

    std::string generateCMake(const std::filesystem::path& currentPath, ryml::NodeRef config, const std::string& realInstallDir) noexcept;

    void runBuildCommand(ryml::NodeRef config, const std::filesystem::path& currentPath, const std::string& name, const std::string& prefix, const std::string& realInstallDir) noexcept;
}
#endif