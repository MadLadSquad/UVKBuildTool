#ifdef UBT_TARGET_ENGINE
#include "CMakeGenerator.hpp"
#include <GeneratorCore.hpp>

template<typename T>
void addFilesToStream(T& stream, const std::vector<UBT::TargetSource>& src, UBT::LibraryProjectType type)
{
    for (const auto& a : src)
    {
        if (a.prjtype & type)
        {
            for (const auto& f : a.glob)
                stream << "\"" << f << "\" ";
            for (const auto& f : a.individual)
                stream << "\"" << f << "\" ";
        }
    }
}

void UBT::generateCmake(const char* name, const CMakeInfoData& data)
{
    UTG::Input in;
    auto result = in.init("../Templates/UntitledVulkanGameEngine/BuildFiles/CMakeLists.txt.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mError when opening the CMakeLists.txt.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["name"] = name;
    std::stringstream ss;
    // -----------------------------------------------------------------------------------------------------------------
    // MSVC include and subdirectories
    // -----------------------------------------------------------------------------------------------------------------
    {
        for (const auto& a : data.msvcIncludePaths)
            ss << "    include_directories(" << a << ")\n";
        in["extra_include_directories_win32"] = ss.str();
        ss.flush();
    }
    {
        for (const auto& a : data.msvcSubdirectories)
            ss << "    add_subdirectory(" << a << ")\n";
        in["extra_subdirectories_win32"] = ss.str();
        ss.flush();
    }
    // -----------------------------------------------------------------------------------------------------------------
    // Unix include and subdirectories-
    // -----------------------------------------------------------------------------------------------------------------
    {
        for (const auto& a : data.unixIncludePaths)
            ss << "    include_directories(" << a << ")\n";
        in["extra_include_directories_unix"] = ss.str();
        ss.flush();
    }
    {
        for (const auto& a : data.unixSubdirectories)
            ss << "    add_subdirectory(" << a << ")\n";
        in["extra_subdirectories_unix"] = ss.str();
        ss.flush();
    }
    // -----------------------------------------------------------------------------------------------------------------
    // MSVC sources and headers
    // -----------------------------------------------------------------------------------------------------------------
    {
        addFilesToStream(ss, data.msvcSources, LIB_FLAGS_LINK_TO_ENGINE);
        in["win32_additional_sources"] = ss.str();
        ss.flush();
    }
    {
        addFilesToStream(ss, data.msvcHeaders, LIB_FLAGS_LINK_TO_ENGINE);
        in["win32_additional_headers"] = ss.str();
        ss.flush();
    }
    {
        addFilesToStream(ss, data.msvcSources, LIB_FLAGS_LINK_TO_WRAPPER);
        in["win32_wrapper_sources"] = ss.str();
        ss.flush();
    }
    {
        addFilesToStream(ss, data.msvcHeaders, LIB_FLAGS_LINK_TO_WRAPPER);
        in["win32_wrapper_headers"] = ss.str();
        ss.flush();
    }
    // -----------------------------------------------------------------------------------------------------------------
    // Unix sources and headers
    // -----------------------------------------------------------------------------------------------------------------
    {
        addFilesToStream(ss, data.unixSources, LIB_FLAGS_LINK_TO_ENGINE);
        in["unix_additional_sources"] = ss.str();
        ss.flush();
    }
    {
        addFilesToStream(ss, data.unixHeaders, LIB_FLAGS_LINK_TO_ENGINE);
        in["unix_additional_headers"] = ss.str();
        ss.flush();
    }
    {
        addFilesToStream(ss, data.unixSources, LIB_FLAGS_LINK_TO_WRAPPER);
        in["unix_wrapper_sources"] = ss.str();
        ss.flush();
    }
    {
        addFilesToStream(ss, data.unixHeaders, LIB_FLAGS_LINK_TO_WRAPPER);
        in["unix_wrapper_headers"] = ss.str();
        ss.flush();
    }
    // -----------------------------------------------------------------------------------------------------------------
    // MSVC modded wrapper sources
    // -----------------------------------------------------------------------------------------------------------------
    {
        addFilesToStream(ss, data.msvcHeaders, LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
        in["win32_modded_wrapper_headers"] = ss.str();
        ss.flush();
    }
    {
        addFilesToStream(ss, data.msvcSources, LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
        in["win32_modded_wrapper_sources"] = ss.str();
        ss.flush();
    }
    // -----------------------------------------------------------------------------------------------------------------
    // Unix modded wrapper sources
    // -----------------------------------------------------------------------------------------------------------------
    {
        addFilesToStream(ss, data.unixHeaders, LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
        in["unix_modded_wrapper_headers"] = ss.str();
        ss.flush();
    }
    {
        addFilesToStream(ss, data.unixSources, LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
        in["unix_modded_wrapper_sources"] = ss.str();
        ss.flush();
    }
    // -----------------------------------------------------------------------------------------------------------------
    // MSVC link libraries
    // -----------------------------------------------------------------------------------------------------------------
    {
        for (const auto& a : data.msvcLinkLibraries)
        {
            if (a.target & LIB_FLAGS_LINK_TO_ENGINE)
            {
                ss << a.val << " ";
            }
        }
        in["win32_engine_link_libraries"] = ss.str();
        ss.flush();
    }
    {
        for (const auto& a : data.msvcLinkLibraries)
        {
            if (a.target  & LIB_FLAGS_LINK_TO_WRAPPER)
            {
                ss << a.val << " ";
            }
        }
        in["win32_wrapper_link_libraries"] = ss.str();
        ss.flush();
    }
    {
        for (const auto& a : data.msvcLinkLibraries)
        {
            if (a.target & LIB_FLAGS_LINK_TO_WRAPPER_MODDED)
            {
                ss << a.val << " ";
            }
        }
        in["win32_wrapper_modded_link_libraries"] = ss.str();
        ss.flush();
    }
    // -----------------------------------------------------------------------------------------------------------------
    // Unix link libraries
    // -----------------------------------------------------------------------------------------------------------------
    {
        for (const auto& a : data.unixLinkLibraries)
        {
            if (a.target & LIB_FLAGS_LINK_TO_ENGINE)
            {
                ss << a.val << " ";
            }
        }
        in["unix_engine_link_libraries"] = ss.str();
        ss.flush();
    }
    {
        for (const auto& a : data.unixLinkLibraries)
        {
            if (a.target  & LIB_FLAGS_LINK_TO_WRAPPER)
            {
                ss << a.val << " ";
            }
        }
        in["unix_wrapper_link_libraries"] = ss.str();
        ss.flush();
    }
    {
        for (const auto& a : data.unixLinkLibraries)
        {
            if (a.target & LIB_FLAGS_LINK_TO_WRAPPER_MODDED)
            {
                ss << a.val << " ";
            }
        }
        in["unix_wrapper_modded_link_libraries"] = ss.str();
        ss.flush();
    }
    // -----------------------------------------------------------------------------------------------------------------
    // End insert section
    // -----------------------------------------------------------------------------------------------------------------
    auto stream = std::ofstream(path + static_cast<std::string>("CMakeLists.txt"));
    stream << in.process() << std::endl;
    stream.close();
}

void UBT::addHeaderLibraries(YAML::Node& config, CMakeInfoData& data)
{
    if (config["additional-header-libraries"])
    {
        auto includes = config["additional-header-libraries"];
        if (includes["msvc"])
        {
            auto target = includes["msvc"];
            data.msvcHeaders.push_back({
                .prjtype = LIB_FLAGS_NONE,
            });

            if (target["engine"])
            {
                data.msvcHeaders.back().prjtype = static_cast<LibraryProjectType>(data.msvcHeaders.back().prjtype | LIB_FLAGS_LINK_TO_ENGINE);
                auto engine = target["engine"];
                accumulateHeaderLibraries(engine, data.msvcHeaders);
            }
            if (target["wrapper"])
            {
                data.msvcHeaders.back().prjtype = static_cast<LibraryProjectType>(data.msvcHeaders.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER);
                auto engine = target["wrapper"];
                accumulateHeaderLibraries(engine, data.msvcHeaders);
            }
            if (target["modded-wrapper"])
            {
                data.msvcHeaders.back().prjtype = static_cast<LibraryProjectType>(data.msvcHeaders.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
                auto engine = target["modded-wrapper"];
                accumulateHeaderLibraries(engine, data.msvcHeaders);
            }
        }
        if (includes["unix"])
        {
            auto target = includes["unix"];
            data.unixHeaders.push_back({
                .prjtype = LIB_FLAGS_NONE,
            });

            if (target["engine"])
            {
                data.unixHeaders.back().prjtype = static_cast<LibraryProjectType>(data.unixHeaders.back().prjtype | LIB_FLAGS_LINK_TO_ENGINE);
                auto engine = target["engine"];
                accumulateHeaderLibraries(engine, data.unixHeaders);
            }
            if (target["wrapper"])
            {
                data.unixHeaders.back().prjtype = static_cast<LibraryProjectType>(data.unixHeaders.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER);
                auto engine = target["wrapper"];
                accumulateHeaderLibraries(engine, data.unixHeaders);
            }
            if (target["modded-wrapper"])
            {
                data.unixHeaders.back().prjtype = static_cast<LibraryProjectType>(data.unixHeaders.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
                auto engine = target["modded-wrapper"];
                accumulateHeaderLibraries(engine, data.unixHeaders);
            }
        }
    }
}

void UBT::addIncludeDirectories(YAML::Node& config, CMakeInfoData& data)
{
    if (config["additional-include-directories"])
    {
        auto includes = config["additional-include-directories"];
        if (includes["msvc"])
            for (const auto& a : includes["msvc"])
                data.msvcIncludePaths.push_back(a.as<std::string>());
        if (includes["unix"])
            for (const auto& a : includes["unix"])
                data.unixIncludePaths.push_back(a.as<std::string>());
    }
}

void UBT::addLinkLibraries(YAML::Node& config, CMakeInfoData& data)
{
    if (config["additional-link-libraries"])
    {
        auto ll = config["additional-link-libraries"];
        if (ll["msvc"])
        {
            for (const auto& target : ll["msvc"])
            {
                UBT::LibraryProjectType type = UBT::LIB_FLAGS_NONE;

                if (target["engine"] && target["engine"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_ENGINE);
                if (target["wrapper"] && target["wrapper"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_WRAPPER);
                if (target["modded-wrapper"] && target["modded-wrapper"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_WRAPPER_MODDED);

                data.msvcLinkLibraries.push_back({
                    .target = type,
                    .val = target["target"].as<std::string>()
                });
            }
        }
        if (ll["unix"])
        {
            for (const auto& target : ll["unix"])
            {
                UBT::LibraryProjectType type = UBT::LIB_FLAGS_NONE;

                if (target["engine"] && target["engine"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_ENGINE);
                if (target["wrapper"] && target["wrapper"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_WRAPPER);
                if (target["modded-wrapper"] && target["modded-wrapper"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_WRAPPER_MODDED);

                data.unixLinkLibraries.push_back({
                    .target = type,
                    .val = target["target"].as<std::string>()
                });
            }
        }
    }
}

void UBT::addSourceLibraries(YAML::Node& config, CMakeInfoData& data)
{
    if (config["additional-source-libraries"])
    {
        auto includes = config["additional-source-libraries"];
        if (includes["msvc"])
        {
            auto target = includes["msvc"];
            data.msvcSources.push_back({
                .prjtype = LIB_FLAGS_NONE,
            });

            if (target["engine"])
            {
                data.msvcSources.back().prjtype = static_cast<LibraryProjectType>(data.msvcSources.back().prjtype | LIB_FLAGS_LINK_TO_ENGINE);
                auto engine = target["engine"];
                accumulateHeaderLibraries(engine, data.msvcSources);
            }
            if (target["wrapper"])
            {
                data.msvcSources.back().prjtype = static_cast<LibraryProjectType>(data.msvcSources.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER);
                auto engine = target["wrapper"];
                accumulateHeaderLibraries(engine, data.msvcSources);
            }
            if (target["modded-wrapper"])
            {
                data.msvcSources.back().prjtype = static_cast<LibraryProjectType>(data.msvcSources.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
                auto engine = target["modded-wrapper"];
                accumulateHeaderLibraries(engine, data.msvcSources);
            }
        }
        if (includes["unix"])
        {
            auto target = includes["unix"];
            data.unixSources.push_back({
                .prjtype = LIB_FLAGS_NONE,
            });

            if (target["engine"])
            {
                data.unixSources.back().prjtype = static_cast<LibraryProjectType>(data.unixSources.back().prjtype | LIB_FLAGS_LINK_TO_ENGINE);
                auto engine = target["engine"];
                accumulateHeaderLibraries(engine, data.unixSources);
            }
            if (target["wrapper"])
            {
                data.unixSources.back().prjtype = static_cast<LibraryProjectType>(data.unixSources.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER);
                auto engine = target["wrapper"];
                accumulateHeaderLibraries(engine, data.unixSources);
            }
            if (target["modded-wrapper"])
            {
                data.unixSources.back().prjtype = static_cast<LibraryProjectType>(data.unixSources.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
                auto engine = target["modded-wrapper"];
                accumulateHeaderLibraries(engine, data.unixSources);
            }
        }
    }
}

void UBT::accumulateHeaderLibraries(YAML::Node& engine, std::vector<TargetSource>& headers)
{
    if (engine["glob"])
        for (const auto& a : engine["glob"])
            headers.back().glob.push_back(a.as<std::string>());
    if (engine["individual"])
        for (const auto& a : engine["individual"])
            headers.back().glob.push_back(a.as<std::string>());
}

void UBT::addSubdirectories(YAML::Node& config, CMakeInfoData& data)
{
    if (config["additional-subdirectories"])
    {
        auto includes = config["additional-subdirectories"];
        if (includes["msvc"])
            for (const auto& a : includes["msvc"])
                data.msvcSubdirectories.push_back(a.as<std::string>());
        if (includes["unix"])
            for (const auto& a : includes["unix"])
                data.unixSubdirectories.push_back(a.as<std::string>());
    }
}
#endif