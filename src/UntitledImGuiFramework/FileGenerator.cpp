#ifdef UBT_TARGET_FRAMEWORK
#include "FileGenerator.hpp"
#include <Generator.hpp>

void UBT::makeTemplate(const std::string& name, const std::string& type, const char* prjname)
{
	bool bInstance = false;
    UTTE::Generator generator{};

	if (type == "Instance") bInstance = true;

    std::string prjnm = prjname;
    for (auto& a : prjnm)
        if (a == '-')
            a = '_';

    UTTE::InitialisationResult result;
    if (bInstance)
    {
        result = generator.loadFromFile("../Templates/UntitledImGuiFramework/GameplayClasses/Instance.hpp.tmpl");
        if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
        {
            std::cout << ERROR << "There was an error when generating the instance file, specifically when opening the Instance.hpp.tmpl file! Error code: " << static_cast<int>(result) << END_COLOUR << std::endl;
            std::terminate();
        }
        generator.pushVariable({ .value = prjnm }, "name");

        auto stream = std::ofstream(path + "Source/Instance.hpp");
        stream << generator.parse().result->c_str();
        stream.close();

        result = generator.loadFromFile("../Templates/UntitledImGuiFramework/GameplayClasses/Instance.cpp.tmpl");
        if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
        {
            std::cout << ERROR << "There was an error when generating the instance file, specifically when opening the Instance.cpp.tmpl file! Error code: " << static_cast<int>(result) << END_COLOUR << std::endl;
            std::terminate();
        }

        stream = std::ofstream(path + "Source/Instance.cpp");
        stream << generator.parse().result->c_str();
        return;
    }

    result = generator.loadFromFile("../Templates/UntitledImGuiFramework/GameplayClasses/GeneratedSource.hpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << ERROR << "There was an error when generating the instance file, specifically when opening the GeneratedSource.hpp.tmpl file! Error code: " << static_cast<int>(result) << END_COLOUR << std::endl;
        std::terminate();
    }
    generator.pushVariable({ .value = prjnm }, "name");
    generator.pushVariable({ .value = name }, "classname");
    generator.pushVariable({ .value = type }, "type");

    auto stream = std::ofstream(path + "Source/" + name + ".hpp");
    stream << generator.parse().result->c_str();
    stream.close();

    result = generator.loadFromFile("../Templates/UntitledImGuiFramework/GameplayClasses/GeneratedSource.cpp.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << ERROR << "There was an error when generating the instance file, specifically when opening the GeneratedSource.cpp.tmpl file! Error code: " << static_cast<int>(result) << END_COLOUR << std::endl;
        std::terminate();
    }

    stream = std::ofstream(path + "Source/" + name + ".cpp");
    stream << generator.parse().result->c_str();
}
#endif