#pragma once
#ifdef UBT_TARGET_FRAMEWORK
#include "Core.hpp"

namespace UBT
{
    UBT_PUBLIC_API void generateMain(const char* gameName);
    UBT_PUBLIC_API void generateDef();
}
#endif