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
        n.load(&bResult);
    else if (alias != nullptr)
    {
        auto nn = node[alias];
        if (!ryml::keyValid(nn))
        {
            PUSH_NONE_VARIABLE(name);
            return;
        }
        nn.load(&bResult);
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
    node["name"].load(&name);
    {
        UTTE::Generator generator{};
        loadTemplate(generator, UBT_TEMPLATES_DIR"/BuildFiles/CMakeLists.txt.tmpl");

        generator.pushVariable({ .value = name }, "name");
        writeTemplate(generator, getPath()/"CMakeLists.txt", UBT_TEMPLATES_DIR"/BuildFiles/CMakeLists.txt.tmpl");
    }
    {
        UTTE::Generator generator{};
        loadTemplate(generator, UBT_TEMPLATES_DIR"/BuildFiles/Modules.cmake.tmpl");

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

        writeTemplate(generator, getPath()/"Generated"/(name + "Modules.cmake"), UBT_TEMPLATES_DIR"/BuildFiles/Modules.cmake.tmpl");
    }
}
#endif
