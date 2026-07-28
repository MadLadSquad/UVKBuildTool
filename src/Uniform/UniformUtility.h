#pragma once
#include <iostream>
#include <ryml.hpp>
#include <ryml_std.hpp>
#include <fstream>
#include <ostream>
#include <filesystem>
#include <Generator.hpp>

// Prefixed because a bare "ERROR" collides with the macro of the same name in windows.h
#define UBT_COL_ERROR "\x1b[31m"
#define UBT_COL_WARNING "\x1b[33m"
#define UBT_COL_SUCCESS "\x1b[32m"
#define UBT_COL_END "\x1b[0m"

namespace ryml
{
    bool keyValid(NodeRef ref) noexcept;
    bool keyValid(ConstNodeRef ref) noexcept;
}

namespace UBT
{
    std::filesystem::path& getPath() noexcept;
    void setPath(const char* pt) noexcept;

    std::string loadFileToString(const std::string& p) noexcept;

    ryml::NodeRef setupWorkdir(const char* x, std::string& name) noexcept;

    ryml::NodeRef getConfig(std::string& name) noexcept;

    // Template helpers.
    //
    // Every one of these reports which template and which destination was involved, because the generators
    // are noexcept: an unchecked failure used to surface either as a crash inside UTTE or as a silently
    // truncated file that only blew up later, inside the user's build.
    //
    // The "try" variants return false so bulk callers(the web variant renders every page of a project) can
    // warn and move on; the plain ones abort, which is what the framework generators want - a project with
    // a half written CMakeLists.txt is not worth continuing with

    bool tryLoadTemplate(UTTE::Generator& generator, const std::string& templatePath) noexcept;
    void loadTemplate(UTTE::Generator& generator, const std::string& templatePath) noexcept;

    // Parses the template currently loaded into the generator, returning nullptr on failure. Worth checking
    // because UTTE returns its buffer either way: a failed parse hands back the half expanded document with
    // only the status to tell them apart, so the bare parse().result-> that this codebase used everywhere
    // wrote a silently corrupt file that only came apart later, in whatever consumed it. "context" names the
    // template in any diagnostic
    const utte_string* tryParseTemplate(UTTE::Generator& generator, const std::string& context) noexcept;

    bool tryWriteTemplate(UTTE::Generator& generator, const std::filesystem::path& destination, const std::string& templatePath) noexcept;
    void writeTemplate(UTTE::Generator& generator, const std::filesystem::path& destination, const std::string& templatePath) noexcept;

    // Filesystem helpers. All of them go through the std::error_code overloads and name the offending path,
    // instead of throwing out of a noexcept function and taking the process down without a diagnostic

    bool tryCreateDirectory(const std::filesystem::path& dir) noexcept;
    void createDirectory(const std::filesystem::path& dir) noexcept;

    void createDirectorySymlink(const std::filesystem::path& target, const std::filesystem::path& link) noexcept;
    void copyFile(const std::filesystem::path& from, const std::filesystem::path& to, std::filesystem::copy_options options = std::filesystem::copy_options::none) noexcept;
}
