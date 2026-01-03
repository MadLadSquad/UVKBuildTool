#include "ReleaseBuildInternal.hpp"
#ifdef UBT_TARGET_FRAMEWORK

void UBT::ReleaseBuildInternal::generateTemporaryBuildDef(
    const std::filesystem::path& currentPath,
    UTTE::Generator& generator,
    UTTE::InitialisationResult& result,
    UTTE::Function** define_or_undefine,
    UTTE::Function** define_or_undefine_dev) noexcept
{
    result = generator.loadFromFile(UBT_TEMPLATES_DIR"/BuildFiles/BuildDef.hpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << ERROR << "There was an error with the generator when generating the BuildDef.hpp file! Error code: " << result << END_COLOUR << std::endl;
        std::terminate();
    }
    *define_or_undefine = &generator.pushVariable({ .value = "#define" }, "define_or_undefine");
    *define_or_undefine_dev = &generator.pushVariable({ .value = "#undef" }, "define_or_undefine_dev");

    std::ofstream out(currentPath/"Generated/BuildDef.hpp");
    out << generator.parse().result->c_str();
    out.close();
}

void UBT::ReleaseBuildInternal::restoreBuildDef(
    const std::filesystem::path& currentPath,
    UTTE::Generator& generator,
    UTTE::InitialisationResult& result,
    UTTE::Function* define_or_undefine,
    UTTE::Function* define_or_undefine_dev) noexcept
{
    result = generator.loadFromFile(UBT_TEMPLATES_DIR"/BuildFiles/BuildDef.hpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << ERROR << "There was an error with the generator when generating the BuildDef.hpp file! Error code: " << result << END_COLOUR << std::endl;
        std::terminate();
    }
    UTTE_VARIABLE_SET_NEW_VAL((*define_or_undefine), result, "#undef", UTTE_VARIABLE_TYPE_HINT_NORMAL);
    UTTE_VARIABLE_SET_NEW_VAL((*define_or_undefine_dev), result, "#undef", UTTE_VARIABLE_TYPE_HINT_NORMAL);

    auto out = std::ofstream(currentPath/"Generated/BuildDef.hpp");
    out << generator.parse().result->c_str();
}
#endif