#pragma once
#ifdef UBT_TARGET_WEB
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <Uniform/UniformUtility.h>
#include "Generator.hpp"
#include <filesystem>

namespace UBT
{
    int showHelp(bool bError) noexcept;
}

#endif