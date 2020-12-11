#pragma once
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <algorithm>
#include <fstream>
#include <ostream>

namespace UBT
{
    std::string toLower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    void GenerateFiles(int subsystem, bool bIsInProduction)
    {
        auto stream = std::ofstream("../../Source/Game.hpp");

        stream << "#include \"Engine/Engine.hpp\"" << std::endl;

        switch(subsystem)
        {
            case 1:
                stream << std::endl;
            case 2:
                stream << "#undef Legacy" << std::endl;
        }

        if(bIsInProduction)
                stream << "#define Production" << std::endl;

        stream.close();
    }
}