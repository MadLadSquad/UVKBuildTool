#ifdef UBT_TARGET_WEB
#include "ConfigManager.hpp"
#include "Functions.hpp"
#include "UBTCustomFunctions/UBTCustomFunctions.hpp"
#include <format>
#include <UI18N.hpp>
#include <exception>

#define SET_ARRAY(x, y) if (node[y]) x = node[y].as<std::vector<std::string>>()

struct GeneratorData
{
    UTTE::Generator generator{};

    std::vector<std::string> allowedExtensions;
    std::vector<std::string> ignoredFiles;
    std::vector<std::string> intermediateFiles;
    std::vector<std::string> localhostCommands;
    std::vector<std::string> customPreGenerationCommands;

    bool bCanUseTranslations = true;
    bool bRunLocalhost = true;
};

void getConfig(const char* path, GeneratorData& data)
{
    YAML::Node node;
    try
    {
        node = YAML::LoadFile(std::string(path) + "/uvproj.yaml");
    }
    catch (YAML::BadFile&)
    {
        std::cout << ERROR <<  "Error: Bad config file!" << END_COLOUR << std::endl;
        std::terminate();
    }

    const auto& variables = node["variables"];
    if (variables)
    {
        for (auto& a : variables)
        {
            if (a["var"] && a["val"])
            {
                auto name = a["var"].as<utte_string>();

                if (a["val"].IsSequence())
                {
                    auto& string = data.generator.requestArrayWithGC();
                    string = a["val"].as<std::vector<utte_string>>();
                    data.generator.pushVariable(UTTE::Generator::makeArray(string), name);
                }
                else if (a["val"].IsMap())
                {
                    auto& map = data.generator.requestMapWithGC();
                    map = a["val"].as<utte_map<utte_string, utte_string>>();
                    data.generator.pushVariable(UTTE::Generator::makeMap(map), name);
                }
                else
                    data.generator.pushVariable({ .value = a["val"].as<utte_string>(), .type = UTTE_VARIABLE_TYPE_HINT_NORMAL }, name);
            }
        }
    }
    SET_ARRAY(data.allowedExtensions,            "allowed-extensions"            );
    SET_ARRAY(data.ignoredFiles,                 "filename-blacklist"            );
    SET_ARRAY(data.intermediateFiles,            "intermediate-extensions"       );
    SET_ARRAY(data.localhostCommands,            "localhost-commands"            );
    SET_ARRAY(data.customPreGenerationCommands,  "custom-pre-generation-commands");

    if (node["run-localhost-automatically"])
        data.bRunLocalhost = node["run-localhost-automatically"].as<bool>();

    data.generator.pushFunction({ .name = "include", .function = UBT::funcInclude });
    if (data.bCanUseTranslations)
    {
        data.generator.pushFunction({ .name = "_", .function = UBT::funcGettext });
        data.generator.pushFunction({ .name = "ui18n_push_global_variable", .function = UBT::funcPushI18NVariable });
    }

}

void generateRecursive(const std::filesystem::path& path, const std::vector<std::string>& allowedExtensions, UTTE::Generator& generator)
{
    for (auto& a : std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::follow_directory_symlink))
    {
        for (auto& f : allowedExtensions)
        {
            if (a.path().string().ends_with(f))
            {
                auto result = generator.loadFromFile(a.path().string());
                if (result == UTTE_INITIALISATION_RESULT_INVALID_FILE)
                {
                    std::cout << WARNING << "Warning: Couldn't generate the following file due to it being invalid: " << a.path().string() << END_COLOUR << std::endl;
                    goto skip_this_file_3;
                }
                std::ofstream out(a.path());
                out << generator.parse().result->c_str();
                out.close();
                goto skip_this_file_3;
            }
        }
skip_this_file_3:;
    }
}

// Copy recursively without copying blacklisted files. std::filesystem::recursive_directory_iterator does not work here
// btw :D
void copyRecursive(const std::filesystem::path& dest, const std::filesystem::path& path, const std::vector<std::string>& ignoredFiles)
{
    if (!exists(dest))
        std::filesystem::create_directory(dest);
    for (auto& a : std::filesystem::directory_iterator(path))
    {
        for (auto& f : ignoredFiles)
            if (a.path().filename() == f)
                goto skip_this_file;

        if (a.is_directory())
        {
            if (a.path() == dest)
                continue;
            copyRecursive(dest/a.path().filename(), a.path(), ignoredFiles);
        }
        else
        {
            std::error_code code;
            std::filesystem::copy_file(a.path(), dest/a.path().filename(), std::filesystem::copy_options::overwrite_existing, code);
        }
skip_this_file:;
    }
}

// Delete intermediate files recursively
void deleteIntermediateRecursive(const std::filesystem::path& path, const std::vector<std::string>& intermediateFiles)
{
    for (auto& a : std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::follow_directory_symlink))
    {
        for (auto& f : intermediateFiles)
        {
            if (a.path().string().ends_with(f))
            {
                std::error_code code;
                std::filesystem::remove(a.path(), code);
                goto skip_this_file_2;
            }
        }
skip_this_file_2:;
    }
}

void buildGeneric(GeneratorData& data,  const std::filesystem::path& ep,
                                        const std::filesystem::path& rootDir,
                                        const std::filesystem::path& projectPath, bool bDeleteIntermediate = false) noexcept
{
    // To prevent file copying with infinite recursion
    data.ignoredFiles.push_back(ep.filename().string());

    // Copy the files to the new directory
    copyRecursive(ep, projectPath, data.ignoredFiles);

    for (auto& a : data.customPreGenerationCommands)
        system(std::format("cd {} && {}", rootDir.string(), a).c_str());

    // Generate the templates recursively
    generateRecursive(ep, data.allowedExtensions, data.generator);

    // Remove any intermediate files
    if (bDeleteIntermediate)
        deleteIntermediateRecursive(ep, data.intermediateFiles);
}

void UBT::buildMain(const char* exportPath, const char* projectPath) noexcept
{
    GeneratorData data{};

    UI18N::TranslationEngine engine{};
    auto result = engine.init((std::string(projectPath) + "/Translations").c_str());
    if (result != UI18N_INIT_RESULT_SUCCESS)
    {
        std::cout << WARNING << "UI18N Warning: Couldn't initialise the translation engine. Error code: " << result << END_COLOUR << std::endl;
        data.bCanUseTranslations = false;
    }
    else
        data.generator.pushVariable({ .value = std::to_string((uintptr_t)&engine), .type = UTTE_VARIABLE_TYPE_HINT_NORMAL,  }, "ui18n_internal_ctx");

    getConfig(projectPath, data);

    // Add custom variables from the user
    UBT::funcExportMain(data.generator);

    std::filesystem::path pp(projectPath);
    for (auto& a : engine.getExistingLocales())
    {
        engine.setCurrentLocale(a);

        std::filesystem::path ep(exportPath);
        ep /= UI18N::languageCodeToString(a);
        rootDir = ep;

        std::filesystem::create_directory(ep);      // Create directory because it will probably not exist

        buildGeneric(data, ep, ep, pp);
    }
    std::filesystem::path ep(exportPath);
    rootDir = ep;
    buildGeneric(data, exportPath, exportPath, pp, false);

    if (data.bRunLocalhost)
        for (auto& a : data.localhostCommands)
            system(std::format("cd {} && {}", rootDir.string(), a).c_str());
}
#endif
