#include "FileGenerator.hpp"
#include <GeneratorCore.hpp>

void UBT::makeTemplate(const std::string& name, const std::string& type, const char* prjname)
{
    bool bAddAutohandles = false;
	bool bGameInstance = false;
    bool bScriptableObject = false;

	if (type == "UVK::Level" || type == "UVK::GameMode" || type == "UVK::PlayerController") bAddAutohandles = true;
	if (type == "UVK::GameInstance") bGameInstance = true;
    if (type == "UVK::ScriptableObject") bScriptableObject = true;

    {
        UTG::Input in;
        std::string tickOrGameInstanceString = ("void UVK::" + name + "::tick(float deltaTime)\n{\n");
        if (bAddAutohandles)
        {
            tickOrGameInstanceString += "    beginAutohandle();";
        }
        tickOrGameInstanceString += "\n\n}";

        if (bGameInstance)
        {
            UTG::Input inst;
            auto result = inst.init("../Templates/GameplayClasses/GameInstanceFunctionDefinitions.tmpl", UTG::Input::INPUT_TYPE_FILE);
            if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
            {
                std::cout << "\x1b[31mThere was an error when generating the game instance file, specifically when opening the GameInstanceFunctionDefinitions.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
                std::terminate();
            }
            inst["name"] = name;
            tickOrGameInstanceString = inst.process();
        }
        else if (bScriptableObject)
        {
            UTG::Input inst;
            auto result = inst.init("../Templates/GameplayClasses/ScriptableObjectFunctionDefinitions.tmpl", UTG::Input::INPUT_TYPE_FILE);
            if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
            {
                std::cout << "\x1b[31mThere was an error when generating the game instance file, specifically when opening the ScriptableObjectFunctionDefinitions.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
                std::terminate();
            }
            inst["name"] = name;
            tickOrGameInstanceString = inst.process();
        }
        auto result = in.init("../Templates/GameplayClasses/GeneratedSource.cpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
        if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
        {
            std::cout << "\x1b[31mThere was an error when generating the game instance file, specifically when opening the GeneratedSource.cpp.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
            std::terminate();
        }
        in["name"] = name;
        if (bAddAutohandles)
        {
            in["add_begin_auto_handle"] = "beginAutohandle();";
            in["add_end_auto_handle"] = "endAutohandle();";
        }
        else
        {
            in["add_begin_auto_handle"] = " ";
            in["add_end_auto_handle"] = " ";
        }
        in["add_tick_or_game_instance_functions"] = tickOrGameInstanceString;

        auto stream = std::ofstream(path + "Source/" + static_cast<std::string>(name) + ".cpp");
        stream << in.process() << std::endl;
        stream.close();
    }
    UTG::Input in;
    std::string tickOrGameInstanceString = "virtual void tick(float deltaTime) override;";
    if (bGameInstance)
    {
        tickOrGameInstanceString = R"(
virtual void onEventInitEditorModules() override;
        virtual void init() override;
        virtual void destroy() override;
)";
    }
    else if (bScriptableObject)
    {
        tickOrGameInstanceString += R"(
        virtual void inactiveBegin() override;
        virtual void inactiveTick(float deltaTime) override;
        virtual void inactiveEnd() override;
)";
    }
    auto result = in.init("../Templates/GameplayClasses/GeneratedSource.hpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mThere was an error when generating the game instance file, specifically when opening the GeneratedSource.hpp.tmpl file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["uppercase_name"] = toUpper(prjname);
    in["name"] = name;
    in["type"] = type;
    in["add_tick_or_game_instance_functions_tick"] = tickOrGameInstanceString;

    auto stream2 = std::ofstream(path + "Source/" + static_cast<std::string>(name) + ".hpp");
    stream2 << in.process() << std::endl;
    stream2.close();
}
