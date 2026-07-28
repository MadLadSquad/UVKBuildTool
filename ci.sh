#!/usr/bin/env bash
cpus=$(grep -c processor /proc/cpuinfo 2> /dev/null) || cpus=$(sysctl -n hw.ncpu)

mkdir -p build || exit 1
cd build || exit 1
cmake .. -DCMAKE_BUILD_TYPE=RELEASE || exit 1
MSBuild.exe UVKBuildTool.sln* -property:Configuration=Release -property:Platform=x64 -property:maxCpuCount="${cpus}" || make -j "${cpus}" || exit 1

echo -e "\x1b[32mSuccessfully built the UVKBuildTool for the UntitledImGuiFramework!\x1b[0m"

cd .. || exit 1
rm -rf build || exit 1
./setup-web.sh . || exit 1
