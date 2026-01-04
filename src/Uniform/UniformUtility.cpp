#include "UniformUtility.h"
#include <Generator.hpp>

std::filesystem::path& UBT::getPath()
{
    static std::filesystem::path path("../../");
    return path;
}

void UBT::setPath(const char* pt)
{
    std::string str = pt;
    if (str.back() != '/')
        str += "/";
    // Cleanup if some stupid Windows user decides to use this
    for (auto& a : str)
        if (a == '\\')
            a = '/';
    getPath() = std::move(std::filesystem::path(str));
}

void UBT::sanitisePath(std::string& s) noexcept
{
#ifdef _WIN32
    for (auto& a : s)
        if (a == '/')
            a = '\\';
#else
    for (auto& a : s)
        if (a == '\\')
            a = '/';
#endif
}

std::string UBT::toUpper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string UBT::loadFileToString(const std::string& p)
{
    std::ifstream in(p);

    in.seekg(0, std::ios::end);
    const size_t size = in.tellg();
    if (size == -1)
        return "";

    std::string buffer(size, ' ');

    in.seekg(0);
    in.read(buffer.data(), static_cast<std::streamsize>(size));
    in.close();
    return buffer;
}

std::string UBT::toLower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

bool ryml::keyValid(NodeRef ref) noexcept
{
    return !ref.invalid() && ref.readable() && !ref.empty();
}

bool ryml::keyValid(const ConstNodeRef ref) noexcept
{
    return !ref.invalid() && ref.readable() && !ref.empty();
}
