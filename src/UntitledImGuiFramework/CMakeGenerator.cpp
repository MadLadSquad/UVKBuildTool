#ifdef UBT_TARGET_FRAMEWORK
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
    auto stream = std::ofstream(path + std::string("CMakeLists.txt"));

    // Windows really likes fucking up everything we do. Basically, if you don't call "->c_str()", a lot of null
    // terminators will be added to the end of the file. After that, Windows will shit itself and would not be able
    // to read the file. Other applications will not be able to open it too.
    //
    // If you're wondering how they don't get removed, it's because the C++ standard defines std::string as a string
    // that is terminated by its size, not the null terminator, unline C strings.
    stream << generator.parse().result->c_str();
}
#endif