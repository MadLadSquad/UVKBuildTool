#pragma once
#ifdef UBT_TARGET_FRAMEWORK
#include "Utility.hpp"

namespace UBT
{
    UBT_PUBLIC_API void generateCmake(const char* name) noexcept;
}
#endif