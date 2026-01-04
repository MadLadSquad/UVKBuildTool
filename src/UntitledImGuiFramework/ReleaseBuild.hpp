#pragma once
#ifdef UBT_TARGET_FRAMEWORK
#include "Core.hpp"

namespace UBT
{
    void relBuild(const std::string& name, ryml::NodeRef config, const std::string& prefix, const std::string& realInstallDir) noexcept;
}
#endif
