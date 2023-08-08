#include "SourceGenerator.hpp"
#include <Generator.hpp>

void UBT::generateMain(const char* gameName)
{
    UTTE::Generator generator{};
    auto result = generator.loadFromFile("../Templates/UntitledImGuiFramework/Sources/main.cpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << "\x1b[31mThere was an error with the generator when generating main.cpp! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    std::string prjnm = gameName;
    for (auto& a : prjnm)
        if (a == '-')
            a = '_';

    generator.pushVariable({ .value = prjnm }, "name");

    auto main = std::ofstream(path + static_cast<std::string>("Generated/main.cpp"));
    main << *generator.parse().result << std::endl;
    main.close();
}

void UBT::generateDef()
{
    UTTE::Generator generator{};
    auto result = generator.loadFromFile("../Templates/UntitledImGuiFramework/BuildFiles/BuildDef.hpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << "\x1b[31mThere was an error with the generator when generating the BuildDef.hpp file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    generator.pushVariable({ .value = "#undef" }, "define_or_undefine");
    generator.pushVariable({ .value = "#undef" }, "define_or_undefine_dev");

    std::ofstream out(path + "Generated/BuildDef.hpp");
    out << *generator.parse().result << std::endl;
    out.close();
}