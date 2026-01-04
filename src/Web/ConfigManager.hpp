#pragma once
#ifdef UBT_TARGET_WEB
#include "Utils.hpp"

namespace UBT
{
    void buildMain(const char* exportPath, const char* projectPath) noexcept;
}
#endif