#pragma once
#ifdef UBT_TARGET_ENGINE
#include "Core.hpp"

namespace UBT
{
    UBT_PUBLIC_API void makeTemplate(const std::string& name, const std::string& type, const char* prjname);
}
#endif