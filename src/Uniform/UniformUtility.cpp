#include "UniformUtility.h"
#include <Generator.hpp>

const std::string& UBT::getPath()
{
    return path;
}

void UBT::setPath(std::string pt)
{
    if (pt.back() != '/')
        pt += "/";
    // Cleanup if some stupid Windows user decides to use this
    for (auto& a : pt)
        if (a == '\\')
            a = '/';
    path = pt;
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

std::string UBT::toLower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}
