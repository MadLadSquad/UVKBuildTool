#ifdef UBT_TARGET_FRAMEWORK
#include "CMakeGenerator.hpp"
#include <Generator.hpp>
#include <exception>

#define PUSH_NONE_VARIABLE(x) generator.pushVariable({ .value = "OFF" }, x)

static void pushVariable(UTTE::Generator& generator, const char* name, const char* alias, ryml::NodeRef node)
{
    auto n = node[name];
    bool bResult{};
    if (ryml::keyValid(n))
        n >> bResult;
    else if (alias != nullptr)
    {
        auto nn = node[alias];
        if (!ryml::keyValid(nn))
        {
            PUSH_NONE_VARIABLE(name);
            return;
        }
        nn >> bResult;
    }
    else
    {
        PUSH_NONE_VARIABLE(name);
        return;
    }
    generator.pushVariable({ .value = bResult ? "ON" : "OFF" }, name);
}

void UBT::generateCmake(ryml::NodeRef node) noexcept
{
    std::string name{};
    node["name"] >> name;
    {
        UTTE::Generator generator{};
        const auto result = generator.loadFromFile(UBT_TEMPLATES_DIR"/BuildFiles/CMakeLists.txt.tmpl");
        if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
        {
            std::cout << ERROR << "Error when opening the CMakeLists.txt.tmpl file! Error code: " << result << END_COLOUR << std::endl;
            std::terminate();
        }

        generator.pushVariable({ .value = name }, "name");
        auto stream = std::ofstream(getPath()/"CMakeLists.txt");

        // Windows really likes fucking up everything we do. Basically, if you don't call "->c_str()", a lot of null
        // terminators will be added to the end of the file. After that, Windows will shit itself and would not be able
        // to read the file. Other applications will not be able to open it too.
        stream << generator.parse().result->c_str();
    }
    {
        UTTE::Generator generator{};
        const auto result = generator.loadFromFile(UBT_TEMPLATES_DIR"/BuildFiles/Modules.cmake.tmpl");
        if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
        {
            std::cout << ERROR << "Error when opening the Modules.cmake.tmpl file! Error code: " << result << END_COLOUR << std::endl;
            std::terminate();
        }

        auto modules = node["enabled-modules"];
        if (ryml::keyValid(modules))
        {
#define PUSH_VARIABLE(x, y) pushVariable(generator, x, y, modules)

            PUSH_VARIABLE("os", nullptr);
            PUSH_VARIABLE("uexec", nullptr);
            PUSH_VARIABLE("open", nullptr);
            PUSH_VARIABLE("xdg", nullptr);
            PUSH_VARIABLE("dbus", nullptr);
            PUSH_VARIABLE("theming", nullptr);
            PUSH_VARIABLE("i18n", nullptr);
            PUSH_VARIABLE("plotting", nullptr);
            PUSH_VARIABLE("knobs", nullptr);
            PUSH_VARIABLE("spinners", nullptr);
            PUSH_VARIABLE("toggles", nullptr);
            PUSH_VARIABLE("undo-redo", "undo_redo");
            PUSH_VARIABLE("cli-parser", "cli_parser");
            PUSH_VARIABLE("text-utils", "text_utils");
        }
        else
        {
            PUSH_NONE_VARIABLE("os");
            PUSH_NONE_VARIABLE("uexec");
            PUSH_NONE_VARIABLE("open");
            PUSH_NONE_VARIABLE("xdg");
            PUSH_NONE_VARIABLE("dbus");
            PUSH_NONE_VARIABLE("theming");
            PUSH_NONE_VARIABLE("i18n");
            PUSH_NONE_VARIABLE("plotting");
            PUSH_NONE_VARIABLE("knobs");
            PUSH_NONE_VARIABLE("spinners");
            PUSH_NONE_VARIABLE("toggles");
            PUSH_NONE_VARIABLE("undo-redo");
            PUSH_NONE_VARIABLE("cli-parser");
            PUSH_NONE_VARIABLE("text-utils");
        }

        auto stream = std::ofstream(getPath()/"Generated"/(name + "Modules.cmake"));
        stream << generator.parse().result->c_str();
    }
}
#endif
