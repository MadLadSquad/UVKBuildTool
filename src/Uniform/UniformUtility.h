#pragma once
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <algorithm>
#include <fstream>
#include <ostream>
#include "parallel-hashmap/parallel_hashmap/phmap.h"
#ifdef _WIN32
    #ifdef UBT_LIB_COMPILE
        #define UBT_PUBLIC_API __declspec(dllexport)
    #else
        #define UBT_PUBLIC_API __declspec(dllimport)
    #endif
#else
    #define UBT_PUBLIC_API
#endif

#define ERROR "\x1b[31m"
#define WARNING "\x1b[33m"
#define SUCCESS "\x1b[32m"
#define END_COLOUR "\x1b[0m"

namespace YAML
{
    // utte_map
    template <typename K, typename V>
    struct convert<phmap::flat_hash_map<K, V>> {
        static Node encode(const phmap::flat_hash_map<K, V>& rhs) {
            Node node(NodeType::Map);
            for (const auto& element : rhs)
                node.force_insert(element.first, element.second);
            return node;
        }

        static bool decode(const Node& node, phmap::flat_hash_map<K, V>& rhs) {
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


namespace UBT
{
    inline std::string path = "../../";

    UBT_PUBLIC_API const std::string& getPath();
    UBT_PUBLIC_API void setPath(std::string pt);

    UBT_PUBLIC_API void sanitisePath(std::string& s) noexcept;

    UBT_PUBLIC_API std::string toLower(std::string str);
    UBT_PUBLIC_API std::string toUpper(std::string str);
}
