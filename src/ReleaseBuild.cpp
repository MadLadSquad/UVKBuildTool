#include "ReleaseBuild.hpp"

void UBT::relBuild(int jobs)
{
    std::ofstream out(path + "Generated/BuildDef.hpp");
    out << "#define PRODUCTION" << std::endl;
    out.close();

    system(static_cast<std::string>("cd " + path + "Exported/ && cmake .. -G \"Unix Makefiles\" && make -j " + std::to_string(jobs) + " && cd ../").c_str());

    system(static_cast<std::string>("cd " + path + "Exported/ && rm -rf CMake* Makefile *.cmake && cd ../").c_str());
    system(static_cast<std::string>("cd " + path + R"(Exported/ && find . -name '*.cmake' -delete && find . -type d -name "CMakeFiles" -exec rm -rf {} \; && find Engine/ -name "Makefile" -exec rm -rf {} \; && find Engine/ -name "*.h" -exec rm -rf {} \; && find Engine/ -name "*.pc" -exec rm -rf {} \; && find . -type d -empty -exec rmdir {} \; && find . -type d -empty -exec rmdir {} \; && cd ../)").c_str());

    std::ofstream out2(path + "Generated/BuildDef.hpp");
    out2 << "// Generated file, DO NOT TOUCH!" << std::endl;
    out2 << "#undef PRODUCTION" << std::endl;
    out2.close();
}