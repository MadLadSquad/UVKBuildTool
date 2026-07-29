#ifdef UBT_TARGET_FRAMEWORK
#include "ReleaseBuild.hpp"
#include "ReleaseBuild/ReleaseBuildInternal.hpp"
#include "SourceGenerator.hpp"
#include <Generator.hpp>
#include <filesystem>
#include <format>
#include <exception>

void UBT::relBuild(const std::string& name, ryml::NodeRef config, const std::string& prefix, const std::string& realInstallDir) noexcept
{
    // Error for conflicting prod export settings
    if (!ReleaseBuildInternal::checkBundleCompatibility(config))
        return;

    const auto currentPath = std::filesystem::path(getPath().c_str());

    // Flip Generated/BuildDef.hpp to a production build for the duration of the build, then put it back.
    // Both directions go through the same generator that --generate uses, so the restored file is byte for
    // byte the development one - the previous version kept raw pointers into the generator's registry
    // across a reload of the template to do this, which was only ever a reallocation away from breaking
    generateDef(true);
    ReleaseBuildInternal::runBuildCommand(
        config,
        currentPath,
        name,
        prefix,
        realInstallDir
    );

    // Both of these undo what the steps above did to the project, so they run whether or not export.sh
    // succeeded: leaving a production BuildDef.hpp or a CMakeLists.txt full of install statements behind
    // would quietly change what the next development build produces
    ReleaseBuildInternal::restoreCMake(currentPath);
    generateDef(false);
}
#endif
