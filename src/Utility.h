#pragma once
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <algorithm>
#include <fstream>
#include <ostream>
#ifdef _WIN32
    #ifdef UBT_LIB_COMPILE
        #define UBT_PUBLIC_API __declspec(dllexport)
    #else
        #define UBT_PUBLIC_API __declspec(dllimport)
    #endif
#else
    #define UBT_PUBLIC_API
#endif


namespace UBT
{
    inline std::string path = "../../";

    UBT_PUBLIC_API static const std::string& getPath()
    {
        return path;
    }

    UBT_PUBLIC_API static void setPath(const char* pt)
    {
        path = pt;
    }

    UBT_PUBLIC_API static std::string toLower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    UBT_PUBLIC_API static std::string toUpper(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }
}
