#include "FileGenerator.hpp"

void UBT::makeTemplate(const std::string& name, const std::string& type)
{
    bool bAddAutohandles = false;
	bool bGameInstance = false;
    bool bScriptableObject = false;

	if (type == "UVK::Level" || type == "UVK::GameMode") bAddAutohandles = true;
	if (type == "UVK::GameInstance") bGameInstance = true;
    if (type == "UVK::ScriptableObject") bScriptableObject = true;

	auto stream = std::ofstream(path + "Source/" + static_cast<std::string>(name) + ".cpp");
    
    stream << "#include " << "\"" << name << ".hpp\"" << std::endl;
    stream << std::endl;
    stream << "void UVK::" << name << "::beginPlay()" << std::endl;
    stream << "{" << std::endl;
    if (bAddAutohandles)
    {
        stream << "    beginAutohandle();" << std::endl;
    }
    stream << std::endl;
    stream << "}" << std::endl;
    stream << std::endl;
	if (!bGameInstance)
	{
		stream << "void UVK::" << name << "::tick(float deltaTime)" << std::endl;
		stream << "{" << std::endl;
		if (bAddAutohandles)
		{
			stream << "    tickAutohandle(deltaTime);" << std::endl;
		}
    	stream << std::endl;

	    stream << "}" << std::endl;
	    stream << std::endl;
    }
    else
    {
    	    stream << "void UVK::" << name << "::onEventInitEditorModules()" << std::endl;
	    stream << "{" << std::endl;
    	    stream << std::endl;
	    stream << "}" << std::endl;
	    stream << std::endl;
    }
    stream << "void UVK::" << name << "::endPlay()" << std::endl;
    stream << "{" << std::endl;
    if (bAddAutohandles)
    {
        stream << "    endAutohandle();" << std::endl;
    }
    stream << std::endl;
    stream << "}" << std::endl;

    if (bScriptableObject)
    {
        stream << "void UVK::" << name << "::inactivebegin()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;

        stream << "void UVK::" << name << "::inactivetick(float deltaTime)" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;

        stream << "void UVK::" << name << "::inactiveend()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;
    }
    stream.close();

    auto stream2 = std::ofstream(path + "Source/" + static_cast<std::string>(name) + ".hpp");
    stream2 << "#pragma once" << std::endl;
    stream2 << "#include \"Game.hpp\"" << std::endl;
    stream2 << std::endl;
    stream2 << "namespace UVK" << std::endl;
    stream2 << "{" << std::endl;
    stream2 << "    class " << name << " : public " << type << std::endl;
    stream2 << "    {" << std::endl;
    stream2 << "    public:" << std::endl;
    stream2 << "        " << name << R"(() 
        {
    
        }

        virtual void endPlay() override;
        virtual void beginPlay() override;)" << std::endl;
    if (!bGameInstance)
    {
        stream2 << "        virtual void tick(float deltaTime) override;" << std::endl;
    }
    else
    {
        stream2 << "        virtual void onEventInitEditorModules() override;" << std::endl;
    }

    if (bScriptableObject)
    {
        stream << R"(
        virtual void inactivebegin() override;
        virtual void inactivetick(float deltaTime) override;
        virtual void inactiveend() override;
)" << std::endl;
    }

    stream2 << "        virtual ~" << name << "() override {}" << R"(
    };
})";
    stream2.close();
}
