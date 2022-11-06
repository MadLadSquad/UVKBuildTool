#include "FileGenerator.hpp"
#include <GeneratorCore.hpp>

void UBT::makeTemplate(const std::string& name, const std::string& type, const char* prjname)
{
	bool bInstance = false;

	if (type == "Instance") bInstance = true;

    UTG::Input in;
    std::string prjnm = prjname;
    for (auto& a : prjnm)
        if (a == '-')
            a = '_';

    if (bInstance)
    {
        auto result = in.init("../Templates/UntitledImGuiFramework/GameplayClasses/Instance.hpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
        if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
        {
            std::cout << "\x1b[31mThere was an error when generating the instance file, specifically when opening the Instance.hpp.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
            std::terminate();
        }
        in["name"] = prjnm;
        auto stream = std::ofstream(path + "Source/Instance.hpp");
        stream << in.process() << std::endl;
        stream.close();

        result = in.init("../Templates/UntitledImGuiFramework/GameplayClasses/Instance.cpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
        if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
        {
            std::cout << "\x1b[31mThere was an error when generating the instance file, specifically when opening the Instance.cpp.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
            std::terminate();
        }
        in["name"] = prjnm;
        stream = std::ofstream(path + "Source/Instance.cpp");
        stream << in.process() << std::endl;
        return;
    }
    auto result = in.init("../Templates/UntitledImGuiFramework/GameplayClasses/GeneratedSource.hpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mThere was an error when generating the instance file, specifically when opening the GeneratedSource.hpp.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["name"] = prjnm;
    in["classname"] = name;
    in["type"] = type;
    auto stream = std::ofstream(path + "Source/" + name + ".hpp");
    stream << in.process() << std::endl;
    stream.close();

    result = in.init("../Templates/UntitledImGuiFramework/GameplayClasses/GeneratedSource.cpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mThere was an error when generating the instance file, specifically when opening the GeneratedSource.cpp.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["name"] = prjnm;
    in["classname"] = name;

    stream = std::ofstream(path + "Source/" + name + ".cpp");
    stream << in.process() << std::endl;
    stream.close();
}
