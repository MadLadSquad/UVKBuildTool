#include "SourceGenerator.hpp"
#include <GeneratorCore.hpp>

void UBT::generateMain(const char* gameName)
{
    UTG::Input in;
    auto result = in.init("../Templates/UntitledImGuiFramework/Sources/main.cpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mThere was an error with the generator when generating main.cpp! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    std::string prjnm = gameName;
    for (auto& a : prjnm)
        if (a == '-')
            a = '_';

    in["name"] = prjnm;

    auto main = std::ofstream(path + static_cast<std::string>("Generated/main.cpp"));
    main << in.process() << std::endl;
    main.close();
}

void UBT::generateDef()
{
    UTG::Input in;
    auto result = in.init("../Templates/UntitledImGuiFramework/BuildFiles/BuildDef.hpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mThere was an error with the generator when generating the BuildDef.hpp file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["define_or_undefine"] = "#undef";
    in["define_or_undefine_dev"] = "#undef";

    std::ofstream out2(path + "Generated/BuildDef.hpp");
    out2 << in.process() << std::endl;
    out2.close();
}
