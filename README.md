# Mathématiques et Graphiques (MAG)

### Prérequis

Pour construire MAG, vous avez besoin des outils suivants :
- **Un compilateur C++20** qui prend en charge C++ modules (clang++-18 ou supérieure)
- **Python 3.11+**
- **Ninja**
- **CMake**

### Option de compilation CMake

- MAG_ENABLE_SIMD
- MAG_BUILD_TESTS
- BUILD_SHARED_LIBS

### Compiler le projet

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

target_link_libraries(${PROJECT_NAME} PRIVATE mag)
target_compile_features(mag PRIVATE cxx_std_<cxx-version>) # You may need to set the target version
```

