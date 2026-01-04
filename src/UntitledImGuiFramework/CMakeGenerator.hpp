#pragma once
#ifdef UBT_TARGET_FRAMEWORK
#include "Utility.hpp"

namespace UBT
{
    void generateCmake(ryml::NodeRef node) noexcept;
}
#endif