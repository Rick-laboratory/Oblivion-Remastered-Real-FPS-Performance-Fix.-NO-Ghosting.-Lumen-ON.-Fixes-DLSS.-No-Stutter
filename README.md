# Oblivion Remastered — Configuration Tool

A Windows C++ utility that fills in a texture-streaming pool size and writes the repository's historical `Engine.ini` preset. It can enumerate DXGI adapters, print the proposed configuration, and back up an existing file before replacing it.

**This is a configuration experiment.** The preset changes hundreds of rendering, streaming, threading and input settings. It is not a verified fix for every game version, GPU, DLSS issue or source of stutter. The repository name is retained for existing links.

## Build

Windows, a C++17 compiler and CMake 3.16+:

```sh
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Examples below use a single-configuration build. Visual Studio builds place the executable in `build/Release/`.

## Inspect and apply

```powershell
.\build\oblivion-config.exe --list-gpus
.\build\oblivion-config.exe --preview --pool-mib 6144
.\build\oblivion-config.exe --preview --adapter 0
.\build\oblivion-config.exe --apply "C:\path\to\Saved\Config\Windows\Engine.ini" --pool-mib 6144
```

Run the game once to create its configuration, then close it. Find the actual `Engine.ini` under your Documents/My Games/Oblivion Remastered configuration directory; the platform folder can be `Windows` or `WinGDK`, and Documents may be redirected. Pass the exact file path. The parent directory must already exist.

- No arguments show help; `--preview` writes the preset to standard output.
- `--apply` **replaces the whole file**, including unrelated custom settings.
- An existing file is copied to a uniquely named sibling `.bak` file. Backup failure stops the operation.
- The replacement is fully written and flushed to a temporary sibling before it replaces the destination. Earlier backups are retained.
- To undo, close the game and restore the printed backup path to the original `Engine.ini`. For a newly created file with no backup, remove that generated file.

Do not edit the same configuration concurrently while applying a preset.

## What the pool value means

Without `--pool-mib`, the tool chooses the adapter with the largest dedicated VRAM value, or the adapter selected by `--adapter`, and retains the original **95% heuristic**. Adapter selection is not detection of the GPU used by the game. Dedicated VRAM is total capacity, not currently available memory; 95% may leave too little space for other GPU resources. Integrated GPUs with no usable dedicated value require an explicit pool override. There is no guessed 4 GiB fallback.

Epic documents [`r.Streaming.PoolSize`](https://dev.epicgames.com/documentation/en-us/unreal-engine/texture-streaming-configuration-in-unreal-engine) as a pool for textures and, on some platforms, additional resources. A pool of zero means unlimited. That definition alone does **not** establish that a particular Oblivion release has a VRAM detection bug. The bundled settings may be ignored or behave differently in the game's engine build.

## Code map

| File | Responsibility |
| --- | --- |
| [main.cpp](main.cpp) | Explicit CLI modes, adapter selection and reporting |
| [gpu.cpp](src/gpu.cpp) | Checked DXGI enumeration and scoped COM ownership |
| [config.cpp](src/config.cpp) | Bounded numeric parsing, substitution and backup/write sequence |
| [Engine.ini.in](presets/Engine.ini.in) | Original two preset blocks, kept in their original order |
| [config_tests.cpp](tests/config_tests.cpp) | Pool bounds, Unicode paths, backups and failed-write preservation |

The preset is embedded at build time. Duplicate settings/sections from the historical profile are preserved; this refresh does not claim to validate each engine variable.

## Validation and downloads

The source builds with MinGW-w64 in C++17 mode. Automated tests cover generated configuration and temporary-directory file operations; GPU enumeration is checked locally. No live game configuration was changed, and no in-game benchmark was performed.

The checked-in `Oblivion Remastered Game Fix.exe` and [original release](https://github.com/Rick-laboratory/Oblivion-Remastered-Real-FPS-Performance-Fix.-NO-Ghosting.-Lumen-ON.-Fixes-DLSS.-No-Stutter/releases/tag/Fix) are historical binaries. They do not include the current backup handling or CLI. Build from source for the behavior documented here.

GPL-3.0; see [LICENSE](LICENSE).
