#ifdef UBT_TARGET_FRAMEWORK
#include "SourceGenerator.hpp"
#include <Generator.hpp>
#include <exception>

void UBT::generateMain(const char* gameName) noexcept
{
    UTTE::Generator generator{};
    const auto result = generator.loadFromFile(UBT_TEMPLATES_DIR"/Sources/main.cpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << ERROR << "There was an error with the generator when generating main.cpp! Error code: " << result << END_COLOUR << std::endl;
        std::terminate();
    }
    std::string prjnm = gameName;
    for (auto& a : prjnm)
        if (a == '-')
            a = '_';

    generator.pushVariable({ .value = prjnm }, "name");

    auto main = std::ofstream(getPath()/"Generated/main.cpp");
    main << generator.parse().result->c_str();
}

void UBT::generateDef() noexcept
{
    UTTE::Generator generator{};
    const auto result = generator.loadFromFile(UBT_TEMPLATES_DIR"/BuildFiles/BuildDef.hpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << ERROR << "There was an error with the generator when generating the BuildDef.hpp file! Error code: " << result << END_COLOUR << std::endl;
        std::terminate();
    }
    generator.pushVariable({ .value = "#undef" }, "define_or_undefine");
    generator.pushVariable({ .value = "#undef" }, "define_or_undefine_dev");

    std::ofstream out(getPath()/"Generated/BuildDef.hpp");
    out << generator.parse().result->c_str();
}
#endif