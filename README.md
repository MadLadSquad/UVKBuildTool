# UVKBuildTool
[![CI](https://github.com/MadLadSquad/UVKBuildTool/actions/workflows/ci.yaml/badge.svg)](https://github.com/MadLadSquad/UVKBuildTool/actions/workflows/ci.yaml)

> Version: 0.8.0.0

The universal file generator, build and release tool for the UntitledVulkanGameEngine and UntitledImGuiFramework, the build tool does the following things:
- Autogenerates build and source files
- Packages the project for production or testing
- Installs some core engine components
- Creates an asset archive for production
- Manages your project files
## It can generate
- CMakeLists.txt files
- Additions to the CMakeLists.txt file such as additional libraries to link with
- UntitledVulkanEngine native scripting header and source files
- A main.cpp file for a UVK project
- Release builds with `GNU make` and `MSVC`
- A content archive for production games - soon
- Encrypted content archive - soon 
## Dependencies
- CMake
- yaml-cpp
 
