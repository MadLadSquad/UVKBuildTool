#pragma once
#ifdef UBT_TARGET_FRAMEWORK
#include "Core.hpp"

namespace UBT
{
    void generateMain(const char* gameName) noexcept;

    // Writes Generated/BuildDef.hpp. The release build flips this to a production definition for the
    // duration of the build and calls it again afterwards to restore the development one
    void generateDef(bool bProduction = false) noexcept;
}
#endif