#pragma once
#ifdef UBT_TARGET_FRAMEWORK
#include <Uniform/UniformUtility.h>

namespace UBT
{
    void generateCmake(ryml::NodeRef node) noexcept;
}
#endif