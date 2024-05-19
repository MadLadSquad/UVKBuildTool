#ifdef UBT_TARGET_WEB
#include "Utils.hpp"
#include <Generator.hpp>

int UBT::showHelp(bool bError) noexcept
{
    UTTE::Generator generator;
    auto result = generator.loadFromFile("../Templates/HelpMessage.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << ERROR << "Error when opening the HelpMessage.tmpl file! Error code: " << static_cast<int>(result) << END_COLOUR << std::endl;
        std::terminate();
    }
    std::string buf;
    std::string tmp;

    std::ifstream instr("../Templates/Web/ArgumentsRegistry.tmpl");
    while (std::getline(instr, tmp))
        buf += tmp + "\n";

    generator.pushVariable({ .value = buf }, "arguments");
    std::cout << *generator.parse().result << std::endl;
    return bError;
}
#endif