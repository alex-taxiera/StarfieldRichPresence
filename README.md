# Starfield Rich Presence

[![C++23](https://img.shields.io/static/v1?label=standard&message=c%2B%2B23&color=blue&logo=c%2B%2B&&logoColor=red&style=flat)](https://en.cppreference.com/w/cpp/compiler_support)
[![Platform](https://img.shields.io/static/v1?label=platform&message=windows&color=dimgray&style=flat&logo=windows)]()
[![Game version](https://img.shields.io/badge/game%20version-1.7.33-orange)]()

## Dependencies

- [CMake v3.27+](https://cmake.org/)
- [vcpkg v2023.08.09+](https://github.com/microsoft/vcpkg/releases)
  - Create a new Windows environment variable called `VCPKG_ROOT` which points to your vcpkg install directory
- [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/) with C++ workload
- [LLVM v17.0.0+](https://github.com/llvm/llvm-project/releases) (not really a dependency but nice to have)

## Building

- Visual Studio should prompt you to generate a CMake cache. Click on `Generate` and wait
- Once the CMake cache is generated, build your project
- The `.dll` and `.pdb` files will be placed in `contrib\PluginRelease` or `contrib\PluginDebug` depending on your build configuration

## Build configs

- Two build configs are provided:
  - `Release`: Optimized release build, produces small and fast DLLs with associated PDBs
  - `Debug`: Debug build, produces DLLs and PDBs with full debug info, allowing the use of an interactive debugger
