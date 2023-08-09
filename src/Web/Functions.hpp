#pragma once
#ifdef UBT_TARGET_WEB
#include "Utils.hpp"

namespace UBT
{
    inline std::filesystem::path rootDir = "./";
    UBT_PUBLIC_API UTTE::Variable funcInclude(std::vector<UTTE::Variable>& args, UTTE::Generator* generator) noexcept;
}
#endif