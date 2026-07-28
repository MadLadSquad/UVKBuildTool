#!/usr/bin/env bash
if [[ $1 == "" ]]; then
  echo "ERROR: The script should be called with a CLI argument that specifies the project path!"
  exit 1
fi

windows=false
vswhere=""

# vswhere ships with every Visual Studio since 2017, so this is only reached on a broken or very old install.
# The download goes to the working directory rather than next to Visual Studio, since writing into
# "Program Files (x86)" needs an elevated shell
function download_vswhere()
{
  echo "vswhere.exe was not found, downloading the official release from Microsoft..."
  curl -fsSL -o vswhere.exe "https://github.com/microsoft/vswhere/releases/latest/download/vswhere.exe" 2> /dev/null && return 0
  powershell -NoProfile -Command "Invoke-WebRequest -Uri 'https://github.com/microsoft/vswhere/releases/latest/download/vswhere.exe' -OutFile 'vswhere.exe'" 2> /dev/null && return 0
  return 1
}

function find_visual_studio_directory()
{
  env | grep "OS=Windows" > /dev/null && windows=true

  if [ "${windows}" = true ]; then
    # Resolved as a path instead of cd-ing into the installer directory: the fallback download lands in the
    # working directory, so a relative "./vswhere.exe" would have looked for it in the wrong place
    vswhere="C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe"
    if [ ! -f "${vswhere}" ]; then
      if ! download_vswhere; then
        echo "ERROR: Couldn't find or download vswhere.exe, which is required to locate your Visual Studio installation!"
        exit 1
      fi
      vswhere="$(pwd)/vswhere.exe"
    fi

    vs_path=$("${vswhere}" | grep "installationPath")
    vs_path="${vs_path:18}"

    VSShortVerLine=$("${vswhere}" | grep "catalog_productLine: Dev")
    VSShortVerLine="${VSShortVerLine#*Dev}"
    VSShortVer="${VSShortVerLine%%[[:space:]]*}"

    VSVer=$(cmake --help 2>&1 | grep "Visual Studio ${VSShortVer} " | grep -oE '[0-9]{4}' | head -1)

    setx PATH "${vs_path}/MSBuild/Current/Bin/amd64/;%PATH%" 2> /dev/null
  fi
  return
}

cpus=$(grep -c processor /proc/cpuinfo 2> /dev/null) || cpus=$(sysctl -n hw.ncpu)

find_visual_studio_directory

cp -r Templates/Web/UBTCustomFunctions "$1/" || exit 1
cp -r "$1/UBTCustomFunctions" src/Web/ || exit 1
mkdir -p "$1/Translations" || exit 1
touch "$1/Translations/ui18n-config.yaml" || exit 1

mkdir -p build || exit 1
cd build || exit 1
if [ "${windows}" = true ]; then
  cmake .. -G "Visual Studio ${VSShortVer} ${VSVer}" -DUBT_COMPILING_FOR_WEB=ON -DCMAKE_BUILD_TYPE=RELEASE || exit 1
  MSBuild.exe UVKBuildTool.sln* -property:Configuration=Release -property:Platform=x64 -property:maxCpuCount="${cpus}" || exit 1
else
  cmake .. -G "Unix Makefiles" -DUBT_COMPILING_FOR_WEB=ON -DCMAKE_BUILD_TYPE=RELEASE || exit 1
  make -j "${cpus}" || exit 1
fi

# Only produced by MSBuild, so on every other platform there is nothing to copy and that is not an error
cp Release/UVKBuildTool.exe . 2> /dev/null || true
cp Release/UVKBuildToolLib.dll . 2> /dev/null || cp Release/libUVKBuildToolLib.dll . 2> /dev/null || true
cp Release/UVKBuildToolLib.lib . 2> /dev/null || cp Release/libUVKBuildToolLib.lib . 2> /dev/null || true

cd ../../ || exit 1
echo -e "\x1b[32mUVKBuildTool set up successfully\x1b[0m"
