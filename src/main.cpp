#include "Utility.h"

int main(int argc, char** argv) {
    if (argv[1] == UBT::toLower("--actor"))
    {
        UBT::GenActor(argv[2]);
        return 0;
    }
    else if (argv[1] == UBT::toLower("--pawn"))
    {
        UBT::GenPawn(argv[2]);
        return 0;
    }
    else if (argv[1] == UBT::toLower("--game-mode"))
    {
        UBT::GenGM(argv[2]);
        return 0;
    }
    else if (argv[1] == UBT::toLower("--game-state"))
    {
        UBT::GenGS(argv[2]);
        return 0;
    }
    else if (argv[1] == UBT::toLower("--player-state"))
    {
        UBT::GenPS(argv[2]);
        return 0;
    }
    else if (argv[1] == UBT::toLower("--player-controller"))
    {
        UBT::GenPC(argv[2]);
        return 0;
    }
    else if (argv[1] == UBT::toLower("--game-instance"))
    {
        UBT::GenGI(argv[2]);
        return 0;
    }
    else if (argv[1] == UBT::toLower("--level"))
    {
        UBT::GenLVL(argv[2]);
        return 0;
    }

    bool bIsInProduction;
    const char* name;
    auto config = YAML::LoadFile("../../uvproj.yaml");

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
            std::cout << "You are trying to use an invalid subsystem! You can only submit legacy or modern as a renderer subsystem! Please fix your uvproj.yaml file!" << std::endl;
        }
    }

    return 0;
}
