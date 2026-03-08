#pragma once
#ifdef UBT_TARGET_WEB
#include <Uniform/UniformUtility.h>
#include <Generator.hpp>

namespace UBT
{
    void buildMain(const char* exportPath, const char* projectPath) noexcept;
}
#endif