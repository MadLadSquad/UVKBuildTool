#ifdef UBT_TARGET_WEB
#include "Functions.hpp"

UTTE::Variable UBT::funcInclude(std::vector<UTTE::Variable>& args, UTTE::Generator* generator) noexcept
{
    if (args.size() == 1)
        return UTTE_ERROR(UTTE_PARSE_STATUS_OUT_OF_BOUNDS);

    UTTE::Variable result{};

    // The include function does recursive parsing of files
    UTTE::Generator gen{};
    gen.getFunctionsRegistry() = generator->getFunctionsRegistry();
    for (size_t i = 1; i < args.size(); i++)
    {
        std::string dt;
        if (gen.loadFromFile(rootDir/args[i].value) == UTTE_INITIALISATION_RESULT_SUCCESS)
            // Note: Your linter is lying to you, this is not redundant. Check out UntitledImGuiFramework/CMakeGenerator.cpp for more info
            result.value += std::string("\n") + gen.parse().result->c_str();
    }
    return result;
}
#endif