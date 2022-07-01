#ifdef UBT_TARGET_ENGINE
#include "ReleaseBuild.hpp"
#include <GeneratorCore.hpp>

void UBT::relBuild(const std::string& name)
{
    // TODO: Add archiving after we are out of Pre-Alpha
    // TODO: Check if DEVELOPMENT was enabled to enable it again
    {
        UTG::Input in;
        auto result = in.init("../Templates/UntitledVulkanGameEngine/BuildFiles/BuildDef.hpp.tmpl", UTG::Input::INPUT_TYPE_FILE);
        if (result != UTG::Input::ERROR_TYPE_NO_ERROR)
        {
            std::cout << "\x1b[31mThere was an error with the generator when generating the BuildDef.hpp file! Error code: " << static_cast<int>(result) << "\x1b[0m" << std::endl;
            std::terminate();
        }
        in["define_or_undefine"] = "#define";
        in["define_or_undefine_dev"] = "#undef";

        std::ofstream out2(path + "Generated/BuildDef.hpp");
        out2 << in.process() << std::endl;
        out2.close();
    }
#ifdef _WIN32
    system(("cd " + UBT::getPath() + " && bash export.sh " + name).c_str());
#else
    system(("cd " + UBT::getPath() + " && ./export.sh " + name).c_str());
#endif
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
#endif