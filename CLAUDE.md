# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

UVKBuildTool (UBT) — the code generator, project manager and release/packaging tool for the
[UntitledImGuiFramework](https://github.com/MadLadSquad/UntitledImGuiFramework). This checkout is normally a submodule
of the framework repo (`UVKBuildTool/`); the framework's own `CLAUDE.md` one directory up documents the framework side
and is the authority on what the generated files mean.

The same source tree builds **two different programs**, selected by the CMake option `UBT_COMPILING_FOR_WEB`:

| | `OFF` (default) | `ON` |
|---|---|---|
| Compile define | `UBT_TARGET_FRAMEWORK` | `UBT_TARGET_WEB` |
| Sources globbed | `src/UntitledImGuiFramework/*` | `src/Web/*` |
| Templates dir | `Templates/UntitledImGuiFramework` | `Templates/Web` |
| Purpose | generate/install/build framework projects, scaffold components | render a templated static website (used for madladsquad.com) |

`src/Uniform/` and the vendored `utg/` (templating engine) are compiled into both. **Every file is *also* wrapped in
`#ifdef UBT_TARGET_FRAMEWORK` / `#elif UBT_TARGET_WEB`** even though the glob already excludes it — keep both guards
when adding files (note the `#elif UBT_TARGET_WEB` form, without `defined()`, is what the codebase uses).

## Build / run

There is no test suite and no linter. CI (`ci.sh`, run on Linux/Windows/macOS) just builds both variants.

```bash
# framework variant (what install.sh/update.sh in the parent repo do)
cmake -B build -DCMAKE_BUILD_TYPE=RELEASE && cmake --build build -j$(nproc)

# web variant
cmake -B build -DCMAKE_BUILD_TYPE=RELEASE -DUBT_COMPILING_FOR_WEB=ON && cmake --build build -j$(nproc)

./ci.sh                              # builds framework variant, wipes build/, then builds web variant
./rebuild.sh <project path> --web|--framework   # rebuild for an existing project
./setup-web.sh <project path>        # first-time web project setup (also seeds UBTCustomFunctions + Translations/)
```

Running it (the binary expects to be invoked from `UVKBuildTool/build/` — the default project path is `../../`):

```bash
cd build
./UVKBuildTool --install   ../../Projects/MyApp                 # first-time project generation (also writes Source/Instance.{hpp,cpp})
./UVKBuildTool --generate  ../../Projects/MyApp                 # regenerate build files, symlinks, Config.hpp
./UVKBuildTool --inline    MyComponent ../../Projects/MyApp     # also --window, --title-bar
./UVKBuildTool --build     <staging prefix> <install prefix> ../../Projects/MyApp
./UVKBuildTool --build     <output dir> <project path>          # web variant: renders the site
```

**`--install` is a create-project-time verb, not a maintenance one.** It is run once, by the framework's
`create-project.sh`, against a directory that script has just made — which is why it may write
`Source/Instance.{hpp,cpp}` unconditionally (there is nothing there to overwrite) and why it does not create
`Generated/`/`Exported/` itself (`create-project.sh` already did: it pre-makes `Source/`, `Generated/`, `build/`
and `Exported/`). Running it by hand against a project you have since written code in **will** overwrite that
project's `Instance.{hpp,cpp}`, and running it against a bare directory fails on the missing `Generated/`. Use
`--generate` to refresh an existing project — that verb is idempotent and creates what it needs.

Every verb is registered **twice** in `src/main.cpp` — once via `pushCommand` (`generate ...`) and once via `pushFlag`
(`--generate ...`, "Compatibility:"). Both point at the *same* callback, which is why the callbacks in
`Commands.hpp` are templates over `T` (= `UCLI::Command` or `UCLI::Flag`). Adding a verb means adding both.

### Compile-time path macros (common source of "it works in my build dir only")

`CMakeLists.txt` bakes absolute paths into the binary:

- `UBT_TEMPLATES_DIR` — templates are read from disk **at runtime**, so editing a `.tmpl` needs no rebuild.
- `UBT_FRAMEWORK_DIR` — realpath of `../` relative to the UVKBuildTool source dir (i.e. the framework root) unless overridden with `-DUBT_FRAMEWORK_DIR=`. Used
  to symlink `Framework/` into a project and to invoke the framework's `export.sh`.
- `UBT_DIR` — where the tool itself lives; symlinked into projects as `UVKBuildTool/`.
- `-DUBT_INSTALL=ON` switches these to `UBT_DATA_INSTALL_PREFIX` (system install: `bin/` + `share/UVKBuildTool/Templates`)
  and additionally defines `UBT_DO_NOT_BUILD_FRAMEWORK`, which makes release builds pass `-DUIMGUI_SKIP_FRAMEWORK=ON`
  (the framework is assumed already installed system-wide).

Because these are absolute, a binary built in one tree cannot drive a project in another.

## Architecture

### Layers

- `src/main.cpp` — UCLI parser wiring only.
- `src/UntitledImGuiFramework/Commands.hpp` — the actual command bodies (`Commands.cpp` is intentionally empty; the
  logic is header-only templates). Each validates `stringValues`, calls `setupWorkdir()`, then delegates to a generator.
- `src/Uniform/UniformUtility.{h,cpp}` — shared by both variants: the global project path (`UBT::getPath()/setPath()`),
  `loadFileToString`, and `getConfig()` which parses `uvproj.yaml`.
- Generators: `CMakeGenerator` (`CMakeLists.txt` + `Generated/<name>Modules.cmake`), `SourceGenerator`
  (`Generated/main.cpp`, `Generated/BuildDef.hpp`), `FileGenerator` (`Source/*.{hpp,cpp}` component/Instance classes).
- `src/UntitledImGuiFramework/ReleaseBuild*` — production packaging (see below).
- `src/Web/` — the static-site generator.

### Config parsing conventions (ryml)

`UBT::getConfig()` holds the parsed tree in **`static` locals** — `uvproj.yaml` is parsed once per process and the
returned `ryml::NodeRef` stays valid for the run. Never assume you can re-read a different project in one invocation.

Always gate node access with the repo's own `ryml::keyValid(node)` helper (checks `!invalid() && readable() && !empty()`)
before `.load(&x)`; a missing key otherwise blows up. Missing values fall back to defaults silently (module flags → `OFF`).

### Templating (UTTE / `utg/`)

All output goes through `UTTE::Generator`: `loadFromFile(UBT_TEMPLATES_DIR"/...")` → `pushVariable({ .value = v }, "name")`
→ `parse()`. Templates use `{{ name }}` placeholders. `utg/CLAUDE.md` documents the engine in depth.

**Always write `generator.parse().result->c_str()` to the stream, never the `std::string` itself.** Writing the string
appends NUL padding that makes the generated file unreadable on Windows. This is repeated (with the explanatory comment)
in `CMakeGenerator.cpp`; `ReleaseBuild/CMake.cpp` additionally scrubs embedded `\0` out of the buffer it appends to.

### Release build pipeline (`--build`)

`UBT::relBuild` (ReleaseBuild.cpp) orchestrates a deliberately reversible sequence:

1. `checkBundleCompatibility` — rejects macOS `bundle: true` together with `build-mode-vendor: false`.
2. `generateDef(true)` (SourceGenerator) — rewrites `Generated/BuildDef.hpp` to `#define PRODUCTION`.
3. `runBuildCommand` → `generateCMake`, which **backs up `CMakeLists.txt` to `CMakeLists.txt.old`** and appends
   generated `if (APPLE)/elseif (WIN32)/else()` install blocks rendered from `CMakeInstall.tmpl`; then shells out to
   the framework's `export.sh` with the computed `-DUIMGUI_INSTALL_PREFIX/-DBUILD_VARIANT_STATIC/-DBUILD_VARIANT_VENDOR/
   -DUIMGUI_INSTALL_FRAMEWORK/-DUIMGUI_SRC_PREFIX` arguments. It first calls `restoreCMake` to recover from an
   interrupted earlier run: a leftover `CMakeLists.txt.old` means the `CMakeLists.txt` beside it still carries that
   run's install statements, which would otherwise be baked into this run's backup and appended to twice.
4. `restoreCMake` — renames `CMakeLists.txt.old` back over `CMakeLists.txt`, so the project is left exactly as it
   was found and a second `--build` behaves like the first.
5. `generateDef(false)` — puts `BuildDef.hpp` back to the development (`#undef`) state, through the very same
   generator `--generate` uses.

Steps 4 and 5 run whether or not `export.sh` succeeded — a failed build must not leave a production `BuildDef.hpp`
or a `CMakeLists.txt` full of install statements behind.

Per-platform install directories are hardcoded structs in `CMake.cpp::getInstallStatements` (Unix `lib64/`+`bin/`,
Windows `Program Files/<name>/…`, macOS `.app` bundle layout when `macos.bundle`), each overridable from the
`install-override:` block, plus `additional-installs:` entries which emit both an `install()` and a
`target_compile_definitions` macro. Config/content dirs shift to `etc/` when installing to `/` and to `../.config/`
when installing under `~/.local`.

### Adding a framework module flag

Three coordinated edits: a `PUSH_VARIABLE("name", <alias or nullptr>)` line **and** a matching
`PUSH_NONE_VARIABLE("name")` in the no-`enabled-modules` fallback branch of `CMakeGenerator.cpp`, plus a
`set(USE_..._MODULE {{ name }})` line in `Templates/UntitledImGuiFramework/BuildFiles/Modules.cmake.tmpl`. The alias
argument exists because some keys accept both `undo-redo` and `undo_redo` spellings.

### Web variant

`UBT::buildMain(exportPath, projectPath)`: initialises `UI18N::TranslationEngine` from `<project>/Translations`, then
for **each existing locale** copies the project into `<exportPath>/<locale>/` and renders it, and finally renders once
more at `<exportPath>/` root. Rendering = copy (skipping `filename-blacklist` +
`UBTCustomFunctions`/`.git`/`UVKBuildTool`) → run `custom-pre-generation-commands` → run UTTE over every file matching
`allowed-extensions` **in place** → delete `intermediate-extensions` → optionally run `localhost-commands`.

The root render resets the engine to the locale `init` was given (`en_US`) first — the per-locale loop leaves the
engine on whichever locale it finished on, and without the reset the "untranslated" root came out as a copy of that
one. The blacklist each render passes to the copy step is a **local copy**: the output directory's own name is added
to it as a recursion guard, and mutating the shared list would leak locale names into every later render.

Template functions available to pages: `include` (recursive parse, resolved relative to the global `UBT::rootDir`),
`_` (gettext-style lookup) and `ui18n_push_global_variable`. The i18n engine pointer is smuggled through the generator
registry as the `ui18n_internal_ctx` pseudo-function (see `getUI18NContext`).

`src/Web/UBTCustomFunctions/` is **gitignored** — it is copied in from `Templates/Web/UBTCustomFunctions/` (or from the
project) by `setup-web.sh`/`rebuild.sh` and is where a site defines `UBT::funcExportMain` to register its own
variables/functions. The web variant therefore cannot be configured without a project path.

## Dependencies

Submodules: `rapidyaml` (config parsing, always vendored), `utg` (UntitledTemplatingEngine, compiled from source),
`ucli` (UntitledCLIParser — a system `pkg-config` copy is preferred on Unix, vendored on Windows), `ui18n`
(UntitledI18N, web variant only). C++23. The code uses rapidyaml (ryml).
