#include "CMakeGenerator.hpp"
#include <GeneratorCore.hpp>

void UBT::generateCmake(const char* name)
{
    UTG::Input in;
    auto result = in.init("../Templates/BuildFiles/CMakeLists.txt.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mError when opening the CMakeLists.txt.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["name"] = name;
    auto stream = std::ofstream(path + static_cast<std::string>("CMakeLists.txt"));
    stream << in.process() << std::endl;
    stream.close();
}