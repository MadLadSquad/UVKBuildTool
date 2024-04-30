#include "UniformUtility.h"
#include <Generator.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML
{
    // utte_map
    template <typename K, typename V>
    struct convert<utte_map<K, V>> {
        static Node encode(const utte_map<K, V>& rhs) {
            Node node(NodeType::Map);
            for (const auto& element : rhs)
                node.force_insert(element.first, element.second);
            return node;
        }

        static bool decode(const Node& node, utte_map<K, V>& rhs) {
            if (!node.IsMap())
                return false;

            rhs.clear();
            for (const auto& element : node)
#if defined(__GNUC__) && __GNUC__ < 4
                // workaround for GCC 3:
                rhs[element.first.template as<K>()] = element.second.template as<V>();
#else
                rhs[element.first.as<K>()] = element.second.as<V>();
#endif
            return true;
        }
    };
}

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
