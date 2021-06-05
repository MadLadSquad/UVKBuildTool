#include "FileGenerator.hpp"

void UBT::makeTemplate(const std::string& name, const std::string& type)
{
    auto stream = std::ofstream(path + "Source/" + static_cast<std::string>(name) + ".cpp");

    stream << "#include " << "\"" << name << ".hpp\"" << std::endl;
    stream << std::endl;
    stream << "void UVK::" << name << "::beginPlay()" << std::endl;
    stream << "{" << std::endl;
    stream << std::endl;
    stream << "}" << std::endl;
    stream << std::endl;
    stream << "void UVK::" << name << "::tick(float deltaTime)" << std::endl;
    stream << "{" << std::endl;
    stream << std::endl;
    stream << "}" << std::endl;
    stream << "void UVK::" << name << "::endPlay()" << std::endl;
    stream << "{" << std::endl;
    stream << std::endl;
    stream << "}" << std::endl;
    stream.close();

    auto stream2 = std::ofstream(path + "Source/" + static_cast<std::string>(name) + ".hpp");
    stream2 << "#include \"Game.hpp\"" << std::endl;
    stream2 << std::endl;
    stream2 << "namespace UVK" << std::endl;
    stream2 << "{" << std::endl;
    stream2 << "    class " << name << " : public " << type << std::endl;
    stream2 << "    {" << std::endl;
    stream2 << "    public:" << std::endl;
    stream2 << "        " << name << "() {}" << std::endl;
    stream2 << "        virtual void tick(float deltaTime) override;" << std::endl;
    stream2 << "        virtual void endPlay() override;" << std::endl;
    stream2 << "        virtual void beginPlay() override;" << std::endl;
    stream2 << "    };" << std::endl;
    stream2 << "}" << std::endl;
    stream2.close();
}