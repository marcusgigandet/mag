# Mathématiques et Graphiques (MAG)

[![macOS Build and Test](https://github.com/SlyFryFrog/mag/actions/workflows/macos_build.yml/badge.svg)](https://github.com/SlyFryFrog/mag/actions/workflows/macos_build.yml)
[![Linux Build and Test](https://github.com/SlyFryFrog/mag/actions/workflows/linux_build.yml/badge.svg)](https://github.com/SlyFryFrog/mag/actions/workflows/linux_build.yml)

### Prérequis

Pour construire MAG, vous avez besoin des outils suivants :

| Tool               | Version              | Description                                          |
|--------------------|----------------------|------------------------------------------------------|
| Compilateur C++20+ | clang++-18 ou g++-15 | Un compilateur C++20 qui prend en charge C++ modules |
| CMake              | 3.28.3               |                                                      |
| Ninja              | N/A                  |                                                      |
| Python             | 3.11+                | Requis si `MAG_NO_DEV_TOOLS` n’est pas défini        |

### Option de compilation CMake

| Option                   | Défaut | Description                                                                                                                                                    |
|--------------------------|--------|----------------------------------------------------------------------------------------------------------------------------------------------------------------|
| MAG_ENABLE_SIMD          | ON     | Active les optimisations SIMD lorsque disponibles et implémente l'API simd                                                                                     |
| MAG_ENABLE_SIMD_EXTENDED | ON     | Active les optimisations SIMD lorqu'elles sont disponibles pour les entiers et les entiers non signés. Cette option nécessite que MAG_ENABLE_SIMD soit active. |
| MAG_CODE_ANALYSIS        | ON     | Active clang-tidy pour analyser le code                                                                                                                        |
| MAG_NO_DEV_TOOLS         | OFF    | Désactive les outils réservés au développement                                                                                                                 |
| MAG_BUILD_TESTS          | OFF    | Active la compilation des tests                                                                                                                                |
| BUILD_SHARED_LIBS        | OFF    | Construire mag en mode partagé                                                                                                                                 |

### Compiling the project - Compiler le projet

```bash
cmake --preset clang-release -DMAG_BUILD_TESTS=off ;
cmake --build --preset clang-release
```

### Utilisation

```cmake
cmake_minimum_required(VERSION 3.28.3) # Minimum cmake for mag
include(FetchContent)

FetchContent_Declare(
        mag
        GIT_REPOSITORY https://github.com/SlyFryFrog/mag.git
        GIT_TAG main
)
FetchContent_MakeAvailable(mag)

target_link_libraries(${PROJECT_NAME} PRIVATE mag::mag)
target_compile_features(mag PRIVATE cxx_std_<cxx-version>) # You may need to set the target version
```

ou

```cmake
cmake_minimum_required(VERSION 3.28.3) # Minimum cmake for mag

add_subdirectory(<path-to-mag>)
target_link_libraries(${PROJECT_NAME} PRIVATE mag::mag)
target_compile_features(mag PRIVATE cxx_std_<cxx-version>) # You may need to set the target version
```

```c++
import mag;

mag::Vec4f vecA;
mag::Vec<double, 4> vecB;
```

## Simd API

```c++
import mag;
using namespace mag;

const f32x4 dt{0.5f};
const f32x4 gravity{9.8f};

Vec4f posVec{1.0f, 2.0f, 3.0f, 4.0f};
Vec4f velVec{0.1f, 0.2f, 0.3f, 0.4f};

f32x4 pos{posVec.v};   // {1.0f, 2.0f, 3.0f, 4.0f}
f32x4 vel{velVec.v};   // {0.1f, 0.2f, 0.3f, 0.4f}

vel += gravity * dt;  // {5.0f, 5.1f, 5.2f, 5.3f}
pos += vel * dt;      // {6.0f, 7.1f. 8.2f, 9.3f}

pos.store(posArr.v);
vel.store(velArr.v);
```