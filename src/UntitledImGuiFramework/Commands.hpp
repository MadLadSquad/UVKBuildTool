#pragma once
#include "../ucli/CLIParser.hpp"

#include <Uniform/UniformUtility.h>
#include "CMakeGenerator.hpp"
#include "FileGenerator.hpp"
#include "SourceGenerator.hpp"
#include "ReleaseBuild.hpp"

namespace UBT
{
    template<typename T>
    UCLI::CallbackResult generateCommand(const T* command) noexcept
    {
        if (command->stringValues.stringValues == nullptr || command->stringValues.stringValuesCount < 1 || command->stringValues.stringValuesCount == SIZE_MAX)
        {
            std::cout << UBT_COL_ERROR << "Invalid argument, generate requires a path to a UVKBuildTool project!" << UBT_COL_END << std::endl << std::endl;
            return UCLI::Parser::helpCommand(command);
        }

        std::string name{};

        auto config = setupWorkdir(command->stringValues.stringValues[0], name);
        const auto path = getPath();

        createDirectory(path/"Exported");
        createDirectory(path/"Generated");

        ensureDirectorySymlink(std::filesystem::path(UBT_FRAMEWORK_DIR)/"Framework", path/"Framework");
        ensureDirectorySymlink(std::filesystem::path(UBT_DIR), path/"UVKBuildTool");

        copyFile(
            std::filesystem::path(UBT_FRAMEWORK_DIR)/"export.sh",
            path/"export.sh",
            std::filesystem::copy_options::overwrite_existing
        );

        generateCmake(config);
        generateMain(name.c_str());
        generateDef();

        copyFile(
            std::filesystem::path(UBT_TEMPLATES_DIR"/Sources/Config.hpp.tmpl"),
            path/"Generated/Config.hpp",
            std::filesystem::copy_options::overwrite_existing
        );

        return UCLI_CALLBACK_RESULT_OK;
    }

    template<typename T>
    UCLI::CallbackResult installCommand(const T* command) noexcept
    {
        if (command->stringValues.stringValues == nullptr || command->stringValues.stringValuesCount < 1 || command->stringValues.stringValuesCount == SIZE_MAX)
        {
            std::cout << UBT_COL_ERROR << "Invalid argument, install requires a path to a UVKBuildTool project!" << UBT_COL_END << std::endl << std::endl;
            return UCLI::Parser::helpCommand(command);
        }

        std::string name{};

        auto config = setupWorkdir(command->stringValues.stringValues[0], name);

        generateCmake(config);
        generateMain(name.c_str());
        generateDef();
        makeTemplate(name + std::string("UIInstance"), "Instance", name.c_str());

        copyFile(
            std::filesystem::path(UBT_TEMPLATES_DIR"/Sources/Config.hpp.tmpl"),
            getPath()/"Generated/Config.hpp"
        );

        return UCLI_CALLBACK_RESULT_OK;
    }

    template<typename T>
    UCLI::CallbackResult buildCommand(const T* command) noexcept
    {
        if (command->stringValues.stringValues == nullptr || command->stringValues.stringValuesCount < 3 || command->stringValues.stringValuesCount == SIZE_MAX)
        {
            std::cout << UBT_COL_ERROR << "Invalid argument, build requires a staging path, installation path and a path to a UVKBuildTool project!" << UBT_COL_END << std::endl << std::endl;
            return UCLI::Parser::helpCommand(command);
        }
        std::string name{};
        auto config = setupWorkdir(command->stringValues.stringValues[2], name);
        UBT::relBuild(name, config, command->stringValues.stringValues[0], command->stringValues.stringValues[1]);
        return UCLI_CALLBACK_RESULT_OK;
    }

    template<typename T>
    UCLI::CallbackResult inlineCommand(const T* command) noexcept
    {
        if (command->stringValues.stringValues == nullptr || command->stringValues.stringValuesCount < 2 || command->stringValues.stringValuesCount == SIZE_MAX)
        {
            std::cout << UBT_COL_ERROR << "Invalid argument, inline requires a component name and a path to a UVKBuildTool project!" << UBT_COL_END << std::endl << std::endl;
            return UCLI::Parser::helpCommand(command);
        }
        std::string name{};
        setupWorkdir(command->stringValues.stringValues[1], name);

        makeTemplate(std::string(command->stringValues.stringValues[0]), "InlineComponent", name.c_str());
        return UCLI_CALLBACK_RESULT_OK;
    }

    template<typename T>
    UCLI::CallbackResult windowCommand(const T* command) noexcept
    {
        if (command->stringValues.stringValues == nullptr || command->stringValues.stringValuesCount < 2 || command->stringValues.stringValuesCount == SIZE_MAX)
        {
            std::cout << UBT_COL_ERROR << "Invalid argument, window requires a component name and a path to a UVKBuildTool project!" << UBT_COL_END << std::endl << std::endl;
            return UCLI::Parser::helpCommand(command);
        }
        std::string name{};
        setupWorkdir(command->stringValues.stringValues[1], name);

        makeTemplate(std::string(command->stringValues.stringValues[0]), "WindowComponent", name.c_str());
        return UCLI_CALLBACK_RESULT_OK;
    }


    template<typename T>
    UCLI::CallbackResult titlebarCommand(const T* command) noexcept
    {
        if (command->stringValues.stringValues == nullptr || command->stringValues.stringValuesCount < 2 || command->stringValues.stringValuesCount == SIZE_MAX)
        {
            std::cout << UBT_COL_ERROR << "Invalid argument, title-bar requires a component name and a path to a UVKBuildTool project!" << UBT_COL_END << std::endl << std::endl;
            return UCLI::Parser::helpCommand(command);
        }
        std::string name{};
        setupWorkdir(command->stringValues.stringValues[1], name);

        makeTemplate(std::string(command->stringValues.stringValues[0]), "TitlebarComponent", name.c_str());
        return UCLI_CALLBACK_RESULT_OK;
    }

}