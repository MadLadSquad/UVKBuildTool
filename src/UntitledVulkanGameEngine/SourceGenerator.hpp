#pragma once
#ifdef UBT_TARGET_ENGINE
#include "Core.hpp"

namespace UBT
{
    UBT_PUBLIC_API void generateMain(const char* startupLevelName, const char* gameName);
    UBT_PUBLIC_API void generateGame(const char* name);
    UBT_PUBLIC_API void generateDef();
    UBT_PUBLIC_API void generateWrapperAndMod();
}
#endif