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
    else if (argv[1] == UBT::toLower("--generate"))
    {
        bool bStartupLevelExists;
        const char* name;
        const char* startupLevelName;
        auto config = YAML::LoadFile("../../uvproj.yaml");

        if (config["startup-level-exists"])
        {
            bStartupLevelExists = config["startup-level-exists"].as<bool>();
        }

        if (config["startup-level"])
        {
            startupLevelName = config["startup-level"].as<std::string>().c_str();
        }

        if (config["name"])
        {
            name = config["name"].as<std::string>().c_str();
        }

        UBT::GenerateFiles(bStartupLevelExists, startupLevelName, name);

        return 0;
    }

}
