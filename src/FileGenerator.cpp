#include "FileGenerator.hpp"

void UBT::makeTemplate(const std::string& name, const std::string& type, const char* prjname)
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
        stream << "void UVK::" << name << "::init()" << std::endl;
        stream << R"({

})";
        stream << "void UVK::" << name << R"()::destroy()
{

}
)";

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
        stream << "void UVK::" << name << "::inactiveBegin()" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;

        stream << "void UVK::" << name << "::inactiveTick(float deltaTime)" << std::endl;
        stream << "{" << std::endl;
        stream << std::endl;
        stream << "}" << std::endl;

        stream << "void UVK::" << name << "::inactiveEnd()" << std::endl;
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
    stream2 << "    class " << UBT::toUpper(prjname) << "_PUBLIC_API " << name << " : public " << type << std::endl;
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
        stream2 << R"(        virtual void onEventInitEditorModules() override;
        virtual void init() override;
        virtual void destroy() override;
)" << std::endl;
    }

    if (bScriptableObject)
    {
        stream2 << R"(
        virtual void inactiveBegin() override;
        virtual void inactiveTick(float deltaTime) override;
        virtual void inactiveEnd() override;
)" << std::endl;
    }

    stream2 << "        virtual ~" << name << "() override {}" << R"(
    };
})";
    stream2.close();
}
