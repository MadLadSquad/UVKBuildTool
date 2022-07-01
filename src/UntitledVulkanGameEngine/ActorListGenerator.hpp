#pragma once
#ifdef UBT_TARGET_ENGINE
#include "Core.hpp"

namespace UBT
{
    UBT_PUBLIC_API void addClass(const std::string& str);
    UBT_PUBLIC_API void removeClass(const std::string& str);
    UBT_PUBLIC_API void generateSet();
}
#endif