#pragma once
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <algorithm>
#include <fstream>
#include <ostream>

#ifndef DEBUG
    static const std::string path = "../../";
#else
    static const std::string path = "./";
#endif

namespace UBT
{
    static std::string toLower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }
}
