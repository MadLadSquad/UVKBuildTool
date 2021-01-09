#include "Utility.h"

int main() {
    YAML::Node config = YAML::LoadFile("../../uvproj.yaml");

    bool bIsInProduction = true;
    const char* name;

    if (config["production"])
    {
        bIsInProduction = config["production"].as<bool>();
    }

    if (config["name"])
    {
        name = config["name"].as<std::string>().c_str();
    }

    if (config["renderer-subsystem"])
    {
        std::string str = UBT::toLower(config["renderer-subsystem"].as<std::string>());

        if (str == "legacy")
        {
            UBT::GenerateFiles(1, bIsInProduction, name);
        }
        else if (str == "modern")
        {
            UBT::GenerateFiles(2, bIsInProduction, name);
        }
        else
        {
            std::cout<<"You are trying to use an invalid subsystem! You can only submit legacy or modern as a renderer subsystem! Please fix your uvproj.yaml file!"<<std::endl;
        }
    }

    return 0;
}
