#!/bin/bash
if [[ $1 == "" ]]; then
  echo "ERROR: The script should be called with a CLI argument that specifies the project path!"
  exit
fi
cpus=$(grep -c processor /proc/cpuinfo) || cpus=$(sysctl -n hw.ncpu)

cp "$1/UBTCustomFunctions" src/Web/ -r || exit

mkdir build &> /dev/null
cd build || exit
cmake .. -DCMAKE_BUILD_TYPE=RELEASE || exit
MSBuild.exe UVKBuildTool.sln -property:Configuration=Release -property:Platform=x64 -property:maxCpuCount="${cpus}" || make -j "${cpus}" || exit

cp Release/UVKBuildTool.exe . 2> /dev/null || echo -n " " || exit
cp Release/UVKBuildToolLib.dll . 2> /dev/null || cp Release/libUVKBuildToolLib.dll . 2> /dev/null || echo -n " "
cp Release/UVKBuildToolLib.lib . 2> /dev/null || cp Release/libUVKBuildToolLib.lib . 2> /dev/null || echo -n " "
cp yaml-cpp/Release/yaml-cpp.dll . 2> /dev/null || cp yaml-cpp/Release/libyaml-cpp.dll . 2> /dev/null || echo -n " "

cd ../../ || exit
echo -e "\x1b[32mUVKBuildTool set up successfully\x1b[0m"
