#pragma once
#ifdef UBT_TARGET_WEB
#include "Utils.hpp"

namespace UBT
{
    inline std::filesystem::path rootDir = "./";
    UTTE::Variable funcInclude(std::vector<UTTE::Variable>& args, UTTE::Generator* generator) noexcept;
    UTTE::Variable funcGettext(std::vector<UTTE::Variable>& args, UTTE::Generator* generator) noexcept;
    UTTE::Variable funcPushI18NVariable(std::vector<UTTE::Variable>& args, UTTE::Generator* generator) noexcept;
}
#endif