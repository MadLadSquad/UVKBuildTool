#ifdef UBT_TARGET_FRAMEWORK
#include "FileGenerator.hpp"
#include <Generator.hpp>
#include <exception>

void UBT::makeTemplate(const std::string& name, const std::string& type, const char* prjname) noexcept
{
    const auto path = getPath();

	bool bInstance = false;
    UTTE::Generator generator{};

	if (type == "Instance") bInstance = true;

    std::string prjnm = prjname;
    for (auto& a : prjnm)
        if (a == '-')
            a = '_';

    if (bInstance)
    {
        loadTemplate(generator, UBT_TEMPLATES_DIR"/UserClasses/Instance.hpp.tmpl");
        generator.pushVariable({ .value = prjnm }, "name");
        writeTemplate(generator, path/"Source/Instance.hpp", UBT_TEMPLATES_DIR"/UserClasses/Instance.hpp.tmpl");

        loadTemplate(generator, UBT_TEMPLATES_DIR"/UserClasses/Instance.cpp.tmpl");
        writeTemplate(generator, path/"Source/Instance.cpp", UBT_TEMPLATES_DIR"/UserClasses/Instance.cpp.tmpl");
        return;
    }

    loadTemplate(generator, UBT_TEMPLATES_DIR"/UserClasses/GeneratedSource.hpp.tmpl");
    generator.pushVariable({ .value = prjnm }, "name");
    generator.pushVariable({ .value = name }, "classname");
    generator.pushVariable({ .value = type }, "type");
    writeTemplate(generator, path/"Source"/(name + ".hpp"), UBT_TEMPLATES_DIR"/UserClasses/GeneratedSource.hpp.tmpl");

    loadTemplate(generator, UBT_TEMPLATES_DIR"/UserClasses/GeneratedSource.cpp.tmpl");
    writeTemplate(generator, path/"Source"/(name + ".cpp"), UBT_TEMPLATES_DIR"/UserClasses/GeneratedSource.cpp.tmpl");
}
#endif