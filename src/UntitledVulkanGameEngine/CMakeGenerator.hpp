#pragma once
#ifdef UBT_TARGET_ENGINE
#include "../Uniform/UniformUtility.h"

namespace UBT
{
    enum LibraryProjectType
    {
        LIB_FLAGS_NONE = 0,
        LIB_FLAGS_LINK_TO_ENGINE = 1 << 0,
        LIB_FLAGS_LINK_TO_WRAPPER = 1 << 1,
        LIB_FLAGS_LINK_TO_WRAPPER_MODDED = 1 << 2,
        LIB_FLAGS_LINK_TO_ALL = LIB_FLAGS_LINK_TO_ENGINE | LIB_FLAGS_LINK_TO_WRAPPER | LIB_FLAGS_LINK_TO_WRAPPER_MODDED
    };

    struct UBT_PUBLIC_API LibSource
    {
        LibraryProjectType target;
        std::string val;
    };

    struct UBT_PUBLIC_API TargetSource
    {
        LibraryProjectType prjtype;
        std::vector<std::string> glob;
        std::vector<std::string> individual;
    };

    struct UBT_PUBLIC_API CMakeInfoData
    {
        std::vector<std::string> msvcIncludePaths;
        std::vector<std::string> unixIncludePaths;

        std::vector<std::string> msvcSubdirectories;
        std::vector<std::string> unixSubdirectories;

        std::vector<TargetSource> msvcSources;
        std::vector<TargetSource> unixSources;

        std::vector<TargetSource> msvcHeaders;
        std::vector<TargetSource> unixHeaders;

        std::vector<LibSource> msvcLinkLibraries;
        std::vector<LibSource> unixLinkLibraries;
    };

    UBT_PUBLIC_API void addIncludeDirectories(YAML::Node& config, CMakeInfoData& data);
    UBT_PUBLIC_API void addLinkLibraries(YAML::Node& config, CMakeInfoData& data);
    UBT_PUBLIC_API void addHeaderLibraries(YAML::Node& config, CMakeInfoData& data);
    UBT_PUBLIC_API void addSourceLibraries(YAML::Node& config, CMakeInfoData& data);
    UBT_PUBLIC_API void addSubdirectories(YAML::Node& config, CMakeInfoData& data);

    UBT_PUBLIC_API void generateCmake(const char* name, const CMakeInfoData& data);

    UBT_PUBLIC_API void accumulateHeaderLibraries(YAML::Node& engine, std::vector<TargetSource>& headers);
}
#endif