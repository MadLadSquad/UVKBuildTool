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
            std::cout << ERROR << "Invalid argument, generate requires a path to a UVKBuildTool project!" << END_COLOUR << std::endl << std::endl;
            return UCLI::Parser::helpCommand(command);
        }

        std::string name{};

        auto config = setupWorkdir(command->stringValues.stringValues[0], name);
        const auto path = getPath();
        if (!std::filesystem::exists(path/"Exported"))
            std::filesystem::create_directory(path/"Exported");
        if (!std::filesystem::exists(path/"Generated"))
            std::filesystem::create_directory(path/"Generated");
        if (!std::filesystem::exists(path/"Framework"))
            std::filesystem::create_directory_symlink(std::filesystem::path(UBT_FRAMEWORK_DIR)/"Framework", path/"Framework");
        if (!std::filesystem::exists(path/"UVKBuildTool"))
            std::filesystem::create_directory_symlink(std::filesystem::path(UBT_DIR), path/"UVKBuildTool");

        std::filesystem::copy_file(
            std::filesystem::path(UBT_FRAMEWORK_DIR)/"export.sh",
            path/"export.sh",
            std::filesystem::copy_options::overwrite_existing
        );

        generateCmake(config);
        generateMain(name.c_str());
        generateDef();

        std::filesystem::copy_file(
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
            std::cout << ERROR << "Invalid argument, install requires a path to a UVKBuildTool project!" << END_COLOUR << std::endl << std::endl;
            return UCLI::Parser::helpCommand(command);
        }

        std::string name{};

        auto config = setupWorkdir(command->stringValues.stringValues[0], name);

        generateCmake(config);
        generateMain(name.c_str());
        generateDef();
        makeTemplate(name + std::string("UIInstance"), "Instance", name.c_str());

        std::filesystem::copy_file(
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
            std::cout << ERROR << "Invalid argument, build requires a staging path, installation path and a path to a UVKBuildTool project!" << END_COLOUR << std::endl << std::endl;
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
            std::cout << ERROR << "Invalid argument, inline requires a component name and a path to a UVKBuildTol project!" << END_COLOUR << std::endl << std::endl;
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
            std::cout << ERROR << "Invalid argument, window requires a component name and a path to a UVKBuildTol project!" << END_COLOUR << std::endl << std::endl;
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
            std::cout << ERROR << "Invalid argument, title-bar requires a component name and a path to a UVKBuildTol project!" << END_COLOUR << std::endl << std::endl;
            return UCLI::Parser::helpCommand(command);
        }
        std::string name{};
        setupWorkdir(command->stringValues.stringValues[1], name);

        makeTemplate(std::string(command->stringValues.stringValues[0]), "TitlebarComponent", name.c_str());
        return UCLI_CALLBACK_RESULT_OK;
    }

}