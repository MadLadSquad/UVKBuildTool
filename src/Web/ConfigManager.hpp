#pragma once
#ifdef UBT_TARGET_WEB
#include "Utils.hpp"

namespace UBT
{
    UBT_PUBLIC_API void buildMain(const char* exportPath, const char* projectPath) noexcept;
}
#endif