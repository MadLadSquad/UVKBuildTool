#!/usr/bin/env bash
if [[ $1 == "" ]]; then
  echo "ERROR: The script should be called with a CLI argument that specifies the project path!"
  exit 1
fi

if [[ $2 == "--web" ]]; then
  web="ON"
elif [[ $2 == "--framework" ]]; then
  web="OFF"
else
  echo "ERROR: The script requires either the '--web' or the '--framework' option, depending on your project type!"
  exit 1
fi

cpus=$(grep -c processor /proc/cpuinfo 2> /dev/null) || cpus=$(sysctl -n hw.ncpu)

# Only web projects have a UBTCustomFunctions directory. Copying it unconditionally made every single
# --framework rebuild fail on a project that was never supposed to have one
if [[ ${web} == "ON" ]]; then
  cp -r "$1/UBTCustomFunctions" src/Web/ || exit 1
fi

mkdir -p build || exit 1
cd build || exit 1
cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DUBT_COMPILING_FOR_WEB="${web}" || exit 1
MSBuild.exe UVKBuildTool.sln* -property:Configuration=Release -property:Platform=x64 -property:maxCpuCount="${cpus}" || make -j "${cpus}" || exit 1

# Only produced by MSBuild, so on every other platform there is nothing to copy and that is not an error
cp Release/UVKBuildTool.exe . 2> /dev/null || true
cp Release/UVKBuildToolLib.dll . 2> /dev/null || cp Release/libUVKBuildToolLib.dll . 2> /dev/null || true
cp Release/UVKBuildToolLib.lib . 2> /dev/null || cp Release/libUVKBuildToolLib.lib . 2> /dev/null || true

cd ../../ || exit 1
echo -e "\x1b[32mUVKBuildTool set up successfully\x1b[0m"
