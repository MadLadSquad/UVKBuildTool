#pragma once
#include <iostream>
#include <ryml.hpp>
#include <ryml_std.hpp>
#include <algorithm>
#include <fstream>
#include <ostream>
#include "parallel-hashmap/parallel_hashmap/phmap.h"
#include <filesystem>

#define ERROR "\x1b[31m"
#define WARNING "\x1b[33m"
#define SUCCESS "\x1b[32m"
#define END_COLOUR "\x1b[0m"

namespace ryml
{
    bool keyValid(NodeRef ref) noexcept;
    bool keyValid(ConstNodeRef ref) noexcept;
}

namespace YAML
{
    // utte_map
    /* TODO:
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
    */
}


namespace UBT
{
    std::filesystem::path& getPath();
    void setPath(const char* pt);

    void sanitisePath(std::string& s) noexcept;

    std::string toLower(std::string str);
    std::string toUpper(std::string str);

    std::string loadFileToString(const std::string& p);
}
