#!/bin/bash
cpus=$(grep -c processor /proc/cpuinfo)

mkdir build
cd build || exit
cmake .. -DCMAKE_BUILD_TYPE=RELEASE || exit
MSBuild.exe UVKBuildTool.sln -property:Configuration=Release -property:Platform=x64 -property:maxCpuCount="${cpus}" || make -j "${cpus}" || exit

echo -e "\x1b[32mSuccessfully built the UVKBuildTool for the UntitledImGuiFramework!\x1b[0m"

cd .. || exit
rm -rf build && mkdir build
cd build || exit

cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DUBT_COMPILING_FOR_WEB=ON || exit
MSBuild.exe UVKBuildTool.sln -property:Configuration=Release -property:Platform=x64 -property:maxCpuCount="${cpus}" || make -j "${cpus}" || exit

echo -e "\x1b[32mSuccessfully built the UVKBuildTool for a web project!\x1b[0m"