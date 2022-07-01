#ifdef UBT_TARGET_ENGINE
#include "SourceGenerator.hpp"
#include <GeneratorCore.hpp>
#include <filesystem>

void UBT::generateGame(const char* name)
{
    auto game = std::ofstream(path + static_cast<std::string>("Source/Game.hpp"));

    UTG::Input in;
    auto result = in.init("../Templates/UntitledVulkanGameEngine/BuildFiles/Game.hpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mThere was an error with the generator when generating Game.hpp! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["uppercase_name"] = toUpper(name);
    game << in.process();
    game.close();
}

void UBT::generateMain(const char* startupLevelName, const char* gameName)
{
    {
        UTG::Input in;
        auto result = in.init("../Templates/UntitledVulkanGameEngine/Sources/main.cpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
        if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
        {
            std::cout << "\x1b[31mThere was an error with the generator when generating main.cpp! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
            std::terminate();
        }
        in["name"] = gameName;
        in["startup_level_name"] = startupLevelName;

        auto main = std::ofstream(path + static_cast<std::string>("Generated/main.cpp"));
        main << in.process() << std::endl;
        main.close();
    }
    UTG::Input in;
    auto result = in.init("../Templates/UntitledVulkanGameEngine/Sources/mainmodded.cpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mThere was an error with the generator when generating the modded main.cpp! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["name"] = gameName;
    in["startup_level_name"] = startupLevelName;

    auto main = std::ofstream(path + static_cast<std::string>("Generated/mainmodded.cpp"));
    main << in.process() << std::endl;
    main.close();
}

void UBT::generateDef()
{
    UTG::Input in;
    auto result = in.init("../Templates/UntitledVulkanGameEngine/BuildFiles/BuildDef.hpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mThere was an error with the generator when generating the BuildDef.hpp file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["define_or_undefine"] = "#undef";
    in["define_or_undefine_dev"] = "#undef";

    std::ofstream out2(path + "Generated/BuildDef.hpp");
    out2 << in.process() << std::endl;
    out2.close();
}

void UBT::generateWrapperAndMod()
{
    std::filesystem::copy_file("../Templates/UntitledVulkanGameEngine/Sources/Wrapper.hpp.tmpl", path + "WrapperSource/Wrapper.hpp");
    std::filesystem::copy_file("../Templates/UntitledVulkanGameEngine/Sources/Wrapper.cpp.tmpl", path + "WrapperSource/Wrapper.cpp");
    std::filesystem::copy_file("../Templates/UntitledVulkanGameEngine/Sources/ModEmpty.hpp.tmpl", path + "Generated/ModEmpty.hpp");
    std::filesystem::copy_file("../Templates/UntitledVulkanGameEngine/Sources/ModEmpty.cpp.tmpl", path + "Generated/ModEmpty.cpp");
}
#endif