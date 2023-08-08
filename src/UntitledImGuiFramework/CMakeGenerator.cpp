#include "CMakeGenerator.hpp"
#include <Generator.hpp>

void UBT::generateCmake(const char* name)
{
    UTTE::Generator generator{};
    auto result = generator.loadFromFile("../Templates/UntitledImGuiFramework/BuildFiles/CMakeLists.txt.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << "\x1b[31mError when opening the CMakeLists.txt.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    generator.pushVariable({ .value = name }, "name");
    auto stream = std::ofstream(path + static_cast<std::string>("CMakeLists.txt"));
    stream << *generator.parse().result;
    stream.close();
}