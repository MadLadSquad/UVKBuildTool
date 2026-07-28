#pragma once
#ifdef UBT_TARGET_WEB
#include "../ucli/CLIParser.hpp"

#include <Uniform/UniformUtility.h>
#include "ConfigManager.hpp"

namespace UBT
{
    // Templated over T so that the same body can back both the "build" command and the "--build"
    // compatibility flag, the same way the framework variant's commands do
    template<typename T>
    UCLI::CallbackResult buildCommand(const T* command) noexcept
    {
        if (command->stringValues.stringValues == nullptr || command->stringValues.stringValuesCount < 2 || command->stringValues.stringValuesCount == SIZE_MAX)
        {
            std::cout << UBT_COL_ERROR << "Invalid argument, build requires an output directory and a path to a UVKBuildTool project!" << UBT_COL_END << std::endl << std::endl;
            return UCLI::Parser::helpCommand(command);
        }
        UBT::buildMain(command->stringValues.stringValues[0], command->stringValues.stringValues[1]);
        return UCLI_CALLBACK_RESULT_OK;
    }
}
#endif
