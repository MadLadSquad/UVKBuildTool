#include "UniformUtility.h"
#include <Generator.hpp>
#include <yaml-cpp/yaml.h>

#define HPMAP_REAL phmap::priv::hash_default_hash<K>, phmap::priv::hash_default_eq<K>, phmap::priv::Allocator<phmap::priv::Pair<const K, V>>

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
