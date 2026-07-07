# MAG

**MAG** (**Mathématiques et Graphiques**) is a modern C++ math library for graphics-oriented 
codebases. It targets **C++20+**, uses **C++ modules**, and provides optional SIMD support.

[![macOS Build and Test](https://github.com/marcusgigandet/mag/actions/workflows/macos_build.yml/badge.svg)](https://github.com/marcusgigandet/mag/actions/workflows/macos_build.yml)
[![Linux Build and Test](https://github.com/marcusgigandet/mag/actions/workflows/linux_build.yml/badge.svg)](https://github.com/marcusgigandet/mag/actions/workflows/linux_build.yml)
[![Windows Build and Test](https://github.com/marcusgigandet/mag/actions/workflows/windows_build.yml/badge.svg)](https://github.com/marcusgigandet/mag/actions/workflows/windows_build.yml)

## Features

- **Single module**: `import mag;`
- **Graphics-focused math**: vectors, matrices, transforms
- **Modern C++ API**: strong typing, modules, concepts
- **Optional SIMD acceleration**: enabled by default on supported architectures

## Quick example

```c++
import mag;
using namespace mag;

Vec4f point{1.0f, 0.0f, 0.0f, 1.0f};

Mat4f model{
    Mat4f::translate(1.0f, 2.0f, 3.0f) *
    Mat4f::rotateZ(pi<float> / 4.0f)
};

Vec4f transformed{model * point};
transformed *= 2;
```

## Adding MAG to a project

```cmake
cmake_minimum_required(VERSION 3.28.3)
project(my_app LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    mag
    GIT_REPOSITORY https://github.com/marcusgigandet/mag.git
    GIT_TAG main
)
FetchContent_MakeAvailable(mag)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE mag::mag)
target_compile_features(my_app PRIVATE cxx_std_20)
```

MAG requires a compiler with usable C++20 module support.

## Documentation

- [Documentation home](https://marcusgigandet.github.io/mag/)

## Module Overview

Mag exposes two modules as part of the library - `mag` and `mag.simd`. If you wish to use the low-level SIMD API, you must import `mag.simd` since the non-simd API, `mag`, only exports the core math API.

## License

Licensed under the [Apache License 2.0](LICENSE).
