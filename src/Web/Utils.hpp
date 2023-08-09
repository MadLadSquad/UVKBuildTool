#pragma once
#ifdef UBT_TARGET_WEB
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <Uniform/UniformUtility.h>
#include "Generator.hpp"
#include <filesystem>

namespace UBT
{
    UBT_PUBLIC_API int showHelp(bool bError) noexcept;
}

#endif