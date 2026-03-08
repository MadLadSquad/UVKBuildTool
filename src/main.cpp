#ifdef UBT_TARGET_FRAMEWORK
    #include <UntitledImGuiFramework/Commands.hpp>
#elif UBT_TARGET_WEB
    #include <Web/ConfigManager.hpp>
    #include <Web/Functions.hpp>
#endif
#include <filesystem>
#include <ryml.hpp>
#include "../ucli/CLIParser.hpp"
#include <exception>

/**
 * @brief Check if an arguments is within bounds and that the args array is not null
 * @arg x - boolean expression to use to check
 * @arg y - Error message
 */
#define CHECK_BOUNDS(x, y) if ((x) || args == nullptr) {      \
    std::cout << ERROR << (y) << END_COLOUR << std::endl;     \
    exit(UBT::showHelp(true));                                \
}

/**
 * @brief Sets the UVKBuildTool project path and gets config
 * @arg x - Project path as string
 */
#define SETUP_WORKDIR(x) UBT::setPath(x); getConfig(config, name);

#ifdef UBT_TARGET_FRAMEWORK
int main(const int argc, char** argv)
{
    UBT::setPath("../../");

    const char* projectPaths[] = { "<path to project>" };
    const char* componentPaths[] = { "<component name>, <path to project>" };
    const char* buildHint[] = { "<staging path>", "<install path>", "<path to project>" };

    UCLI::Parser parser{};
    parser
        .setHelpHeader("UVKBuildTol - The universal file generator for the UntitledImGuiFramework")
        .setHelpFooter("Copyright (c) MadLadSquad")
        .pushCommand({
            .longName = "generate",
            .shortName = 'g',
            .description = "Regenerates all required files for the given project",

            .defaultValues = projectPaths,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_STRING,

            .subcommands = nullptr,
            .subcommandsCount = 0,

            .flags = nullptr,
            .flagsCount = 0,

            .callback = UBT::generateCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushCommand({
            .longName = "install",
            .shortName = 'i',
            .description = "Generates the project files when installing for the first time",

            .defaultValues = projectPaths,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_STRING,

            .subcommands = nullptr,
            .subcommandsCount = 0,

            .flags = nullptr,
            .flagsCount = 0,

            .callback = UBT::installCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushCommand({
            .longName = "build",
            .shortName = 'b',
            .description = "Bundles the application and compiles it for production",

            .defaultValues = buildHint,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_ARRAY,

            .subcommands = nullptr,
            .subcommandsCount = 0,

            .flags = nullptr,
            .flagsCount = 0,

            .callback = UBT::buildCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushCommand({
            .longName = "inline",
            .shortName = 0,
            .description = "Generates an inline component",

            .defaultValues = componentPaths,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_ARRAY,

            .subcommands = nullptr,
            .subcommandsCount = 0,

            .flags = nullptr,
            .flagsCount = 0,

            .callback = UBT::inlineCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushCommand({
            .longName = "window",
            .shortName = 0,
            .description = "Generates a window component",

            .defaultValues = componentPaths,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_ARRAY,

            .subcommands = nullptr,
            .subcommandsCount = 0,

            .flags = nullptr,
            .flagsCount = 0,

            .callback = UBT::windowCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushCommand({
            .longName = "title-bar",
            .shortName = 0,
            .description = "Generates a title bar component",

            .defaultValues = componentPaths,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_ARRAY,

            .subcommands = nullptr,
            .subcommandsCount = 0,

            .flags = nullptr,
            .flagsCount = 0,

            .callback = UBT::titlebarCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushFlag({
            .longName = "generate",
            .shortName = 'g',
            .description = "Compatibility: Regenerates all required files for the given project",

            .defaultValues = projectPaths,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_STRING,

            .callback = UBT::generateCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushFlag({
            .longName = "install",
            .shortName = 'i',
            .description = "Compatibility: Generates the project files when installing for the first time",

            .defaultValues = projectPaths,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_STRING,

            .callback = UBT::installCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushFlag({
            .longName = "build",
            .shortName = 'b',
            .description = "Compatibility: Bundles the application and compiles it for production",

            .defaultValues = buildHint,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_ARRAY,

            .callback = UBT::buildCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushFlag({
            .longName = "inline",
            .shortName = 0,
            .description = "Compatibility: Generates an inline component",

            .defaultValues = componentPaths,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_ARRAY,

            .callback = UBT::inlineCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushFlag({
            .longName = "window",
            .shortName = 0,
            .description = "Compatibility: Generates a window component",

            .defaultValues = componentPaths,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_ARRAY,

            .callback = UBT::windowCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushFlag({
            .longName = "title-bar",
            .shortName = 0,
            .description = "Compatibility: Generates a title bar component",

            .defaultValues = componentPaths,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_ARRAY,

            .callback = UBT::titlebarCommand,
            .context = &parser,

            .useLiteralFlags = false
        })
        .parse(argc, argv);

    return 0;
}
#elif UBT_TARGET_WEB
int main(int argc, char** argv)
{
    UBT::setPath("../../");

    const char* defaultValues[] = { "<output directory>", "<project path>" };
    UCLI::Parser parser{};
    parser
        .setHelpHeader("UVKBuildTol - The universal file generator for static websites")
        .setHelpFooter("Copyright (c) MadLadSquad")
        .pushCommand(UCLI::Command{
            .longName = "build",
            .shortName = 'b',
            .description = "Builds the project to the specified output directory",

            .defaultValues = defaultValues,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_ARRAY,

            .subcommands = nullptr,
            .subcommandsCount = 0,

            .flags = nullptr,
            .flagsCount = 0,

            .callback = [](const UCLI::Command* command) -> UCLI::CallbackResult
            {
                if (command->stringValues.stringValues == nullptr || command->stringValues.stringValuesCount < 2 || command->stringValues.stringValuesCount == SIZE_MAX)
                {
                    std::cout << ERROR << "Invalid argument, generate requires a path to a UVKBuildTool project!" << END_COLOUR << std::endl << std::endl;
                    return UCLI::Parser::helpCommand(command);
                }
                UBT::buildMain(command->stringValues.stringValues[0], command->stringValues.stringValues[1]);
                return UCLI_CALLBACK_RESULT_OK;
            },
            .context = &parser,

            .useLiteralFlags = false
        })
        .pushFlag(UCLI::Flag{
            .longName = "build",
            .shortName = 'b',
            .description = "Builds the project to the specified output directory",

            .defaultValues = defaultValues,
            .defaultValuesCount = SIZE_MAX,

            .boolValue = nullptr,
            .type = UCLI_COMMAND_TYPE_ARRAY,

            .callback = [](const UCLI::Flag* command) -> UCLI::CallbackResult
            {
                if (command->stringValues.stringValues == nullptr || command->stringValues.stringValuesCount < 2 || command->stringValues.stringValuesCount == SIZE_MAX)
                {
                    std::cout << ERROR << "Invalid argument, generate requires a path to a UVKBuildTool project!" << END_COLOUR << std::endl << std::endl;
                    return UCLI::Parser::helpCommand(command);
                }
                UBT::buildMain(command->stringValues.stringValues[0], command->stringValues.stringValues[1]);
                return UCLI_CALLBACK_RESULT_OK;
            },
            .context = &parser,

            .useLiteralFlags = false
        })
        .parse(argc, argv);
}
#endif
