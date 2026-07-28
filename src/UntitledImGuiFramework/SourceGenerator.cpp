#ifdef UBT_TARGET_FRAMEWORK
#include "SourceGenerator.hpp"
#include <Generator.hpp>
#include <exception>

void UBT::generateMain(const char* gameName) noexcept
{
    UTTE::Generator generator{};
    loadTemplate(generator, UBT_TEMPLATES_DIR"/Sources/main.cpp.tmpl");

    std::string prjnm = gameName;
    for (auto& a : prjnm)
        if (a == '-')
            a = '_';

    generator.pushVariable({ .value = prjnm }, "name");
    writeTemplate(generator, getPath()/"Generated/main.cpp", UBT_TEMPLATES_DIR"/Sources/main.cpp.tmpl");
}

void UBT::generateDef(const bool bProduction) noexcept
{
    UTTE::Generator generator{};
    loadTemplate(generator, UBT_TEMPLATES_DIR"/BuildFiles/BuildDef.hpp.tmpl");

    generator.pushVariable({ .value = bProduction ? "#define" : "#undef" }, "define_or_undefine");
    generator.pushVariable({ .value = "#undef" }, "define_or_undefine_dev");

    writeTemplate(generator, getPath()/"Generated/BuildDef.hpp", UBT_TEMPLATES_DIR"/BuildFiles/BuildDef.hpp.tmpl");
}
#endif