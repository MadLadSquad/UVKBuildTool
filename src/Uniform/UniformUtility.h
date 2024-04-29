#pragma once
#define UTTE_CUSTOM_MAP phmap::flat_hash_map
#define UTTE_CUSTOM_MAP_INCLUDE <Uniform/parallel-hashmap/parallel_hashmap/phmap.h>
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


namespace UBT
{
    inline std::string path = "../../";

    UBT_PUBLIC_API const std::string& getPath();
    UBT_PUBLIC_API void setPath(std::string pt);

    UBT_PUBLIC_API std::string toLower(std::string str);
    UBT_PUBLIC_API std::string toUpper(std::string str);
}
