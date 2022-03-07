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

    UBT_PUBLIC_API const std::string& getPath();
    UBT_PUBLIC_API void setPath(std::string pt);

    UBT_PUBLIC_API std::string toLower(std::string str);
    UBT_PUBLIC_API std::string toUpper(std::string str);
}
