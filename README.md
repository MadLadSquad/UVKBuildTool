# UVKBuildTool
[![CI](https://github.com/MadLadSquad/UVKBuildTool/actions/workflows/ci.yaml/badge.svg)](https://github.com/MadLadSquad/UVKBuildTool/actions/workflows/ci.yaml)
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)
[![trello](https://img.shields.io/badge/Trello-UVK-blue])](https://trello.com/b/0upjsxT0/untitledvukangameengine)
[![Discord](https://img.shields.io/discord/717037253292982315.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](https://discord.gg/4wgH8ZE)

> Version: 0.8.0.2

The universal file generator, build and release tool for the 
[UntitledVulkanGameEngine](https://github.com/MadLadSquad/UntitledVulkanGameEngine),
[UntitledImGuiFramework](https://github.com/MadLadSquad/UntitledImGuiFramework) and for web applications like
[Youyin](https://youyin.madladsquad.com/)

For the UntitledVulkanGameEngine and UntitledImGuiFramework, the application does the following:
- Autogenerates build and source files
- Packages the project for production or testing
- Installs some core framework components
- Creates an asset archive for production
- Manages your project files

For web applications like Youyin, it does the following:
- Allows for a components based architecture by utilising static templates
- Generates HTML, CSS and JS files and deploys them using your preferred web server
- Allows for translating a static webpage into multiple languages
- Minifies HTML, CSS and JS files for production deployment

## Features
### UntitledVulkanGameEngine
- [X] Generate project files such as `CMakeLists.txt` and `main.cpp`
- [X] Generate native scripting header and source files
- [X] Create release builds
- [ ] Create a content archive for production deployment

### UntitledImGuiFramework
- [X] Generate project files such as `CMakeLists.txt` and `main.cpp`
- [X] Generate UI components header and source files
- [X] Create release builds

### Web
- [X] Generate pages from templated html files(for creating a component type system)
- [X] Host pages locally for development using your preferred web server
- [X] Minifies HTML, CSS and JS files for production deployment

## Documentation
Documentation can be found on the [wiki](https://github.com/MadLadSquad/UVKBuildTool/wiki)

## Dependencies
- CMake
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
