# Mathématiques et Graphiques (MAG)

[![macOS Build and Test](https://github.com/marcusgigandet/mag/actions/workflows/macos_build.yml/badge.svg)](https://github.com/marcusgigandet/mag/actions/workflows/macos_build.yml)
[![Linux Build and Test](https://github.com/marcusgigandet/mag/actions/workflows/linux_build.yml/badge.svg)](https://github.com/marcusgigandet/mag/actions/workflows/linux_build.yml)
[![Windows Build and Test](https://github.com/marcusgigandet/mag/actions/workflows/windows_build.yml/badge.svg)](https://github.com/marcusgigandet/mag/actions/workflows/windows_build.yml)

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
        GIT_REPOSITORY https://github.com/marcusgigandet/mag.git
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

## Exemples d'utilisation

### Vecteurs

```c++
import mag;
using namespace mag;

Vec4f a{1.0f, 2.0f, 3.0f, 4.0f};
Vec4f b{3.0f, 4.0f, 5.0f, 6.0f};

auto sum = a + b;
auto scaled = 0.5f * a;

Vec4f c = a;
c += b;
c -= 1.0f;

float len = a.length();
float d = distance(a, b);
auto lerped = lerp(a, b, 0.25f);
```

### Matrices

```c++
import mag;
using namespace mag;

Mat4f model =
		Mat4f::translate(1.0f, 2.0f, 3.0f) *
		Mat4f::rotateZ(pi<float> / 4.0f);

Vec4f p{1.0f, 0.0f, 0.0f, 1.0f};
Vec4f transformed = model * p;

Mat4f invModel = model.inverse();
Mat4f identity = model * invModel;

Mat4f diag = Mat4f::diagonal(Vec4f{2.0f, 3.0f, 4.0f, 1.0f});
```

### API SIMD

```c++
#include <array>
#include <span>

import mag;
using namespace mag;
using namespace mag::simd;

std::array<float, 4> data{5.0f, 6.0f, 7.0f, 8.0f};

f32x4 a{2.0f, -4.0f, 8.0f, -16.0f};
f32x4 b{1.0f, 2.0f, -4.0f, -8.0f};

auto simdResult = (a + b) * 0.5f;   // Simd & scalar ops
auto scalarLeft = 2.0f - b;

f32x4 fromPtr{data.data()};
f32x4 fromSpan{std::span<const float, 4>{data}};
auto fixedLoaded = load<float, fixed_abi<4>>(data.data());
auto splatted = splat<float, fixed_abi<4>>(9.0f);

float sum = hsum(a);
float minVal = hmin(a);
float maxVal = hmax(a);

auto lo = min(a, b);
auto hi = max(a, b);

std::array<float, 4> out{};
simdResult.store(out.data());
```
