# Welcome to MAG

**MAG (Mathematics and Graphics)** is a modern-cpp math library built for use in 3D graphics.
The library targets **C++20 and newer**, uses **C++ modules**, and ships a compact API for
vector math, matrix math, transform helpers, constants, and optional SIMD acceleration.

## Features

- A single public module import: `import mag;`
- Vector types for 2D, 3D, and 4D math
- Matrix types and transform helpers for graphics workflows
- Optional SIMD types and helpers when SIMD support is enabled at build time
- CMake integration for both in-tree and dependency-based builds

## Start here

- [Library overview and module layout](getting-started/overview.md)
- [Configure, build, and test MAG](getting-started/building.md)
- [Add MAG to another CMake project](getting-started/integration.md)
- [Basic vector, matrix, and SIMD examples](getting-started/using-mag.md)

```{toctree}
:hidden:
:caption: Getting Started

getting-started/overview
getting-started/building
getting-started/integration
getting-started/using-mag
```

```{toctree}
:hidden:
:caption: Reference

reference/code-reference
```

```{toctree}
:hidden:
:caption: About

about/changelog
```
