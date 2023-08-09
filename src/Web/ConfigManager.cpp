#ifdef UBT_TARGET_WEB
#include "ConfigManager.hpp"
#include "Functions.hpp"
#include "UBTCustomFunctions/UBTCustomFunctions.hpp"

#define SET_ARRAY(x, y) if (node[y]) x = node[y].as<std::vector<std::string>>()

void getConfig(const char* path, UTTE::Generator& generator, std::vector<std::string>& allowedExt,
                                                             std::vector<std::string>& ignoredFiles,
                                                             std::vector<std::string>& intermediateFiles,
                                                             std::vector<std::string>& localhostCommands, bool& bRunAutomatically) noexcept
{
    YAML::Node node;
    try
    {
        node = YAML::LoadFile(std::string(path) + "/uvproj.yaml");
    }
    catch (YAML::BadFile&)
    {
        std::cout << "Error: Bad config file!" << std::endl;
        std::terminate();
    }

    const auto& variables = node["variables"];
    if (variables)
    {
        for (auto& a : variables)
        {
            if (a["var"] && a["val"])
            {
                auto name = a["var"].as<std::string>();

                if (a["val"].IsSequence())
                {
                    auto& string = generator.requestArrayWithGC();
                    string = a["val"].as<std::vector<std::string>>();
                    generator.pushVariable(UTTE::Generator::makeArray(string), name);
                }
                else if (a["val"].IsMap())
                {
                    auto& map = generator.requestMapWithGC();
                    map = a["val"].as<std::map<std::string, std::string>>();
                    generator.pushVariable(UTTE::Generator::makeMap(map), name);
                }
                else
                    generator.pushVariable({ .value = a["val"].as<std::string>(), .type = UTTE_VARIABLE_TYPE_HINT_NORMAL }, name);
            }
        }
    }
    SET_ARRAY(allowedExt,           "allowed-extensions"        );
    SET_ARRAY(ignoredFiles,         "filename-blacklist"        );
    SET_ARRAY(intermediateFiles,    "intermediate-extensions"   );
    SET_ARRAY(localhostCommands,    "localhost-commands"        );

    if (node["run-localhost-automatically"])
        bRunAutomatically = node["run-localhost-automatically"].as<bool>();

    generator.pushFunction({ .name = "include", .function = UBT::funcInclude });
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
                    std::cout << "Warning: Couldn't generate the following file due to it being invalid: " << a.path().string() << std::endl;
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

void UBT::buildMain(const char* exportPath, const char* projectPath) noexcept
{
    std::vector<std::string> allowedExtensions;
    std::vector<std::string> ignoredFiles;
    std::vector<std::string> intermediateFiles;
    std::vector<std::string> localhostCommands;
    bool bRunLocalhost = true;

    UTTE::Generator generator{};
    getConfig(projectPath, generator, allowedExtensions, ignoredFiles, intermediateFiles, localhostCommands, bRunLocalhost);

    // Add custom variables from the user
    UBT::funcExportMain(generator);

    std::filesystem::path ep(exportPath);
    rootDir = ep;

    // Copy the files to the new directory
    copyRecursive(ep, std::filesystem::path(projectPath), ignoredFiles);

    // Generate the templates recursively
    generateRecursive(ep, allowedExtensions, generator);

    // Remove any intermediate files
    deleteIntermediateRecursive(ep, intermediateFiles);

    if (bRunLocalhost)
        for (auto& a : localhostCommands)
            system(("cd " + rootDir.string() + " && " + a).c_str());
}
#endif
