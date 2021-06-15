#include "ReleaseBuild.hpp"

void UBT::relBuild(int jobs)
{
    std::ofstream out(path + "Generated/BuildDef.hpp");
    out << "#define PRODUCTION" << std::endl;
    out.close();

    system(static_cast<std::string>("cd " + path + "Exported/ && cmake .. -G \"Unix Makefiles\" && make -j " + std::to_string(jobs) + " && cd ../").c_str());

    std::ofstream out2(path + "Generated/BuildDef.hpp");
    out2 << "// Generated file, DO NOT TOUCH!" << std::endl;
    out2 << "#undef PRODUCTION" << std::endl;
    out2.close();
}