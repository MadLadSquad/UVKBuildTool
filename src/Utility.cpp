#include "Utility.h"

const std::string& UBT::getPath()
{
    return path;
}

void UBT::setPath(std::string pt)
{
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