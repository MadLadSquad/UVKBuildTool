#pragma once
#ifdef UBT_TARGET_FRAMEWORK
#include "Core.hpp"

namespace UBT
{
    void generateMain(const char* gameName) noexcept;
    void generateDef() noexcept;
}
#endif