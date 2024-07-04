#ifdef UBT_TARGET_FRAMEWORK
#include "CMakeGenerator.hpp"
#include <Generator.hpp>

void UBT::generateCmake(const YAML::Node& node) noexcept
{
    {
        UTTE::Generator generator{};
        const auto result = generator.loadFromFile("../Templates/UntitledImGuiFramework/BuildFiles/CMakeLists.txt.tmpl");
        if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
        {
            std::cout << ERROR << "Error when opening the CMakeLists.txt.tmpl file! Error code: " << result << END_COLOUR << std::endl;
            std::terminate();
        }
        generator.pushVariable({ .value = node["name"].as<std::string>() }, "name");
        auto stream = std::ofstream(getPath() + std::string("CMakeLists.txt"));

        // Windows really likes fucking up everything we do. Basically, if you don't call "->c_str()", a lot of null
        // terminators will be added to the end of the file. After that, Windows will shit itself and would not be able
        // to read the file. Other applications will not be able to open it too.
        //
        // If you're wondering how they don't get removed, it's because the C++ standard defines std::string as a string
        // that is terminated by its size, not the null terminator, unline C strings.
        stream << generator.parse().result->c_str();
    }
    {
        UTTE::Generator generator{};
        const auto result = generator.loadFromFile("../Templates/UntitledImGuiFramework/BuildFiles/Modules.cmake.tmpl");
        if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
        {
            std::cout << ERROR << "Error when opening the Modules.cmake.tmpl file! Error code: " << result << END_COLOUR << std::endl;
            std::terminate();
        }
        auto modules = node["enabled-modules"];
#define PUSH_VARIABLE(x) generator.pushVariable({ .value = (modules[#x] && modules[#x].as<bool>()) ? "ON" : "OFF" }, #x)

        PUSH_VARIABLE(os);
        PUSH_VARIABLE(uexec);
        PUSH_VARIABLE(ufont);
        PUSH_VARIABLE(open);
        PUSH_VARIABLE(xdg);
        PUSH_VARIABLE(dbus);

        PUSH_VARIABLE(theming);
        PUSH_VARIABLE(i18n);
        PUSH_VARIABLE(undo-redo);
        PUSH_VARIABLE(cli-parser);

        PUSH_VARIABLE(plotting);
        PUSH_VARIABLE(knobs);
        PUSH_VARIABLE(spinners);
        PUSH_VARIABLE(toggles);
        PUSH_VARIABLE(text-utils);

        auto stream = std::ofstream(getPath() + "Generated/" + node["name"].as<std::string>() + "Modules.cmake");
        stream << generator.parse().result->c_str();
    }
}
#endif