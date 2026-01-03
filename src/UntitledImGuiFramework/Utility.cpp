#ifdef UBT_TARGET_FRAMEWORK
#include "Utility.hpp"
#include <Generator.hpp>
#include <exception>

int UBT::showHelp(bool bError) noexcept
{
    UTTE::Generator generator;
    const auto result = generator.loadFromFile(UBT_TEMPLATES_DIR"/../HelpMessage.tmpl");
    if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
    {
        std::cout << ERROR << "Error when opening the HelpMessage.tmpl file! Error code: " << result << END_COLOUR << std::endl;
        std::terminate();
    }
    std::string buf;
    std::string tmp;

    std::ifstream instr(UBT_TEMPLATES_DIR"/ArgumentsRegistry.tmpl");
    while (std::getline(instr, tmp))
        buf += tmp + "\n";

    generator.pushVariable({ .value = buf }, "arguments");
    std::cout << *generator.parse().result << std::endl;
    return bError;
}
#endif
