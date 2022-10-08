# UVKBuildTool
[![CI](https://github.com/MadLadSquad/UVKBuildTool/actions/workflows/ci.yaml/badge.svg)](https://github.com/MadLadSquad/UVKBuildTool/actions/workflows/ci.yaml)
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)
[![trello](https://img.shields.io/badge/Trello-UVK-blue])](https://trello.com/b/0upjsxT0/untitledvukangameengine)
[![Discord](https://img.shields.io/discord/717037253292982315.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](https://discord.gg/4wgH8ZE)

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
 
