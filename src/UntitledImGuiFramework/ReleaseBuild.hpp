#pragma once
#ifdef UBT_TARGET_FRAMEWORK
#include "Core.hpp"

namespace UBT
{
    UBT_PUBLIC_API void relBuild(const std::string& name, YAML::Node& config, const std::string& prefix, const std::string& realInstallDir) noexcept;
}
#endif
