#include "ActorListGenerator.hpp"
#include <GeneratorCore.hpp>
#include <filesystem>

void UBT::generateSet()
{
    UTG::Input in;
    auto result = in.init("../Templates/BuildFiles/ActorList.hpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mError when generating the ActorList file from template! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["custom_include"] = "";
    in["sources"] = "";


    auto stream = std::ofstream(path + "Generated/ActorList.hpp");
    stream << in.process() << std::endl;
    stream.close();
}

void UBT::addClass(const std::string& str)
{
    // A temporary variable to hold the temporary string during file iteration
    std::string it;
    std::string headers;
    std::string sources;
    std::filesystem::path loc = str;
    bool bCollectingHeaders = false;
    bool bCollectingSources = false;

    auto stream = std::ifstream(path + "Generated/ActorList.hpp");
    size_t i = 0;
    while (std::getline(stream, it))
    {
        i++;
        if (it.empty())
            continue;
        if (!bCollectingHeaders && it.find("#include \"../Engine/GameFramework/Actors/ScriptableObject.hpp\"") != std::string::npos)
        {
            bCollectingHeaders = true;
            continue;
        }
        if (bCollectingHeaders)
        {
            if (it.find("#define ACTOR_SET_DEPLOY \\") != std::string::npos)
            {
                bCollectingHeaders = false;
                bCollectingSources = true;
                continue;
            }
            else if (it.empty() || it == "\n")
                continue;
            headers += it + "\n";
        }
        if (bCollectingSources)
        {
            if (it.empty() || it == "\n")
                break;
            sources += it + "\n";
        }
    }
    headers += "#include \"" + loc.string() + "\" \n";
    sources += "UVK::" + loc.stem().string() + "* " + loc.stem().string() + "auto = new UVK::" + loc.stem().string() + R"((); \
a.emplace_back()" + loc.stem().string() + R"(auto);)";

    UTG::Input in;
    auto result = in.init("../Templates/BuildFiles/ActorList.hpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
    if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
    {
        std::cout << "\x1b[31mError when opening the ActorList.hpp.tmpl! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
        std::terminate();
    }
    in["custom_includes"] = headers;
    in["sources"] = sources;

    auto out = std::ofstream(path + "Generated/ActorList.hpp");
    out << in.process() << std::endl;
    out.close();
}

void UBT::removeClass(const std::string& str)
{
    std::string it;
    std::string buffer;
    std::filesystem::path location = str;

    std::ifstream in(path + "Generated/ActorList.hpp");

    const std::string include = R"(#include ")" + location.string() + "\"";
    const std::string emplace = R"(a.emplace_back()" + location.stem().string() + "auto);";
    const std::string donew = "UVK::" + location.stem().string() + "* " + location.stem().string() + "auto = new UVK::" + location.stem().string() + "();";

    while (std::getline(in, it))
    {
        if (it.find(include) != std::string::npos || it.find(emplace) != std::string::npos || it.find(donew) != std::string::npos)
            continue;
        buffer.append(it + '\n');
        it.clear();
    }

    in.close();

    auto out = std::ofstream(path + "Generated/ActorList.hpp");
    out << buffer << std::endl;
    out.close();

    in = std::ifstream(path + "Generated/ActorList.hpp");
    buffer.clear();
    while (std::getline(in, it))
    {
        if (it.empty())
            continue;
        buffer += it + '\n';
    }
    in.close();

    out = std::ofstream(path + "Generated/ActorList.hpp");
    out << buffer << std::endl;
    out.close();
}
