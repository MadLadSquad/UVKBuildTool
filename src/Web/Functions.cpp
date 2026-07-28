#ifdef UBT_TARGET_WEB
#include "Functions.hpp"
#include <UI18N.hpp>
#include <cstring>
#include <charconv>
#include <cstdint>

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
        const auto included = (rootDir/args[i].value).string();
        if (gen.loadFromFile(included) == UTTE_INITIALISATION_RESULT_SUCCESS)
        {
            // An included page that fails to parse is skipped with a diagnostic naming it, rather than
            // pasting the half expanded buffer UTTE hands back on failure into the including page
            const auto* parsed = UBT::tryParseTemplate(gen, included);
            if (parsed != nullptr)
                // Note: Your linter is lying to you, this is not redundant. Check out UntitledImGuiFramework/CMakeGenerator.cpp for more info
                result.value += std::string("\n") + parsed->c_str();
        }
    }
    return result;
}

UI18N::TranslationEngine* getUI18NContext(UTTE::Generator* generator) noexcept
{
    UI18N::TranslationEngine* engine = nullptr;
    for (auto& a : generator->getFunctionsRegistry())
    {
        if (a.name == "ui18n_internal_ctx")
        {
            std::vector<UTTE::Variable> tmp = { { .value = "ui18n_internal_ctx" } };
            const auto address = a.function(tmp, generator).value;

            // std::from_chars instead of std::stoll: the value round-trips a std::uintptr_t, which a signed
            // conversion would throw on above INT64_MAX - out of a call chain that is entirely noexcept
            std::uintptr_t pointer = 0;
            if (std::from_chars(address.data(), address.data() + address.size(), pointer).ec != std::errc{})
                break;

            engine = reinterpret_cast<UI18N::TranslationEngine*>(pointer);
            break;
        }
    }
    return engine;
}

UTTE::Variable UBT::funcGettext(std::vector<UTTE::Variable>& args, UTTE::Generator* generator) noexcept
{
    if (args.size() == 1)
        return UTTE_ERROR(UTTE_PARSE_STATUS_OUT_OF_BOUNDS);

    auto* engine = getUI18NContext(generator);
    if (engine == nullptr)
        return UTTE_ERROR(UTTE_PARSE_STATUS_INVALID_VALUE);

    std::vector<ui18nstring> positionalArgs{};
    ui18nmap<ui18nstring, ui18nstring> mapArgs{};

    // Add additional arguments
    for (size_t i = 2; i < args.size(); i++)
    {
        if (args[i].type == UTTE_VARIABLE_TYPE_HINT_ARRAY)
        {
            auto* tmp = UTTE::CoreFuncs::getArray(args[i]);
            positionalArgs.insert(positionalArgs.end(), tmp->begin(), tmp->end());
        }
        else if (args[i].type == UTTE_VARIABLE_TYPE_HINT_MAP)
        {
            auto* tmp = UTTE::CoreFuncs::getMap(args[i]);
            mapArgs.insert(tmp->begin(), tmp->end());
        }
        else
            positionalArgs.push_back(args[i].value); // If it's not an array or map just push to the array
    }
    return { .value = engine->get(args[1].value.c_str(), positionalArgs, mapArgs), .type = UTTE_VARIABLE_TYPE_HINT_NORMAL, .status = UTTE_PARSE_STATUS_SUCCESS };
}

UTTE::Variable UBT::funcPushI18NVariable(std::vector<UTTE::Variable>& args, UTTE::Generator* generator) noexcept
{
    if (args.size() == 1)
        return UTTE_ERROR(UTTE_PARSE_STATUS_OUT_OF_BOUNDS);

    auto* engine = getUI18NContext(generator);
    if (engine == nullptr)
        return UTTE_ERROR(UTTE_PARSE_STATUS_INVALID_VALUE);

    for (size_t i = 1; i < args.size() && (i + 1) < args.size(); i += 2)
        engine->pushVariable(args[i].value, args[i + 1].value);
    return UTTE_ERROR(UTTE_PARSE_STATUS_SUCCESS);
}
#endif