#include "ActorListGenerator.hpp"

void UBT::generateSet()
{
    auto stream = std::ofstream(path + "Generated/ActorList.hpp");
    stream << R"(// AUTOGENERATED FILE DO NOT TOUCH IF YOU DON'T KNOW WHAT YOU ARE DOING
#pragma once
#include "../Engine/GameFramework/Actors/ScriptableObject.hpp"

#define ACTOR_SET_DEPLOY \

)" << std::endl;
    stream.close();
}

void UBT::addClass(const std::string& str)
{
    // A lot of memory is being wasted here but who cares
    std::string it;
    std::string buffer;
    std::string className = str;
    std::string loc = str;
    bool bAddedHeader = false;
    bool bEncounteredSet = false;

    className.erase(className.find_last_of('.'));
    if (className.find('/') < className.size())
    {
        className.erase(0, className.find_last_of('/') + 1);
    }

    std::string toErase = path + "Generated/";
    auto a = loc.find(toErase);
    if (a != std::string::npos)
    {
        loc.erase(a, toErase.length());
    }

    auto stream = std::ifstream(path + "Generated/ActorList.hpp");
    while (std::getline(stream, it))
    {
        if (it.empty() && !bAddedHeader)
        {
            it = "#include \"" + loc + "\" \n";
            //className = "UVK::" + className;
            buffer.append(it + "\n");
            bAddedHeader = true;
            continue;
        }

        if (it.find("#define ACTOR_SET_DEPLOY") != std::string::npos) bEncounteredSet = true;

        if (bAddedHeader && it.empty() && bEncounteredSet)
        {
            auto a = className;
            className = "UVK::" + className;
            it = className + "* " + a + "ABC = new " + className + R"((); \
a.emplace()" + a + "ABC" + "); \\\n";
            buffer.append(it + "\n");
            break;
        }

        buffer.append(it + "\n");
        it.clear();
    }

    stream.close();

    auto out = std::ofstream(path + "Generated/ActorList.hpp");
    out << buffer << std::endl;
    out.close();
}

void UBT::removeClass(const std::string& str)
{
    std::string it;
    std::string buffer;
    std::string loc = str;
    std::string className = str;

    className.erase(className.find_last_of('.'));
    if (className.find('/') < className.size())
    {
        className.erase(0, className.find_last_of('/') + 1);
    }

    std::string toErase = path + "Generated/";
    auto a = loc.find(toErase);
    if (a != std::string::npos)
    {
        loc.erase(a, toErase.length());
    }

    std::ifstream in(path + "Generated/ActorList.hpp");

    while (std::getline(in, it))
    {
        if (it.find("#include \"" + loc + "\"") != std::string::npos || it.find("a.emplace(" + className + "ABC)") != std::string::npos || it.find("UVK::" + className + "* " + className + "ABC = new UVK::") != std::string::npos)
        {
            it.clear();
            buffer.append(it + '\n');
            continue;
        }
        buffer.append(it + '\n');
        it.clear();
    }

    in.close();

    auto out = std::ofstream(path + "Generated/ActorList.hpp");
    out << buffer << std::endl;
    out.close();
}
