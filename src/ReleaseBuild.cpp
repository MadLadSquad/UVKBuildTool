#include "ReleaseBuild.hpp"

void UBT::relBuild(const std::string& name)
{
    // TODO: Add archiving after we are out of Pre-Alpha
    std::ofstream out(path + "Generated/BuildDef.hpp");
    out << "#define PRODUCTION" << std::endl;
    out.close();

#ifdef _WIN32
    system(("cd ../../ && bash export.sh " + name).c_str());
#else
    system(("cd ../../ && ./export.sh " + name).c_str());
#endif
    std::ofstream out2(path + "Generated/BuildDef.hpp");
    out2 << "// Generated file, DO NOT TOUCH!" << std::endl;
    out2 << "#undef PRODUCTION" << std::endl;
    out2.close();
}