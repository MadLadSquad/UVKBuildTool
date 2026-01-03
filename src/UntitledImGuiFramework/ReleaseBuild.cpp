#ifdef UBT_TARGET_FRAMEWORK
#include "ReleaseBuild.hpp"
#include "ReleaseBuild/ReleaseBuildInternal.hpp"
#include <Generator.hpp>
#include <filesystem>
#include <format>
#include <exception>

void UBT::relBuild(const std::string& name, YAML::Node& config, const std::string& prefix, const std::string& realInstallDir) noexcept
{
    // Error for conflicting prod export settings
    if (!ReleaseBuildInternal::checkBundleCompatibility(config))
        return;

    const auto currentPath = std::filesystem::path(getPath().c_str());

    UTTE::Generator generator{};
    UTTE::InitialisationResult result;

    UTTE::Function* define_or_undefine = nullptr;
    UTTE::Function* define_or_undefine_dev = nullptr;

    ReleaseBuildInternal::generateTemporaryBuildDef(currentPath, generator, result, &define_or_undefine, &define_or_undefine_dev);
    ReleaseBuildInternal::runBuildCommand(
        config,
        currentPath,
        name,
        prefix,
        realInstallDir
    );
    ReleaseBuildInternal::restoreBuildDef(currentPath, generator, result, define_or_undefine, define_or_undefine_dev);
}
#endif
