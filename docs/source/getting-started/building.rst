Building from Source
====================

Build MAG with CMake and the repository presets.

Prerequisites
-------------

Install the following tools before configuring the project:

- A modern C++ compiler with C++20 modules support:

  - **Clang 18+**
  - **GCC 15+**
  - **MSVC 19.40+**
  - **CMake 3.28.3+**
  - **Ninja**
  - **Python 3** when development tooling is enabled

The bundled presets assume Ninja and create build directories under ``build/``.

Quick start with presets
------------------------

Choose the preset that matches your toolchain:

+----------------+----------------------------+
| Preset         | Typical use                |
+================+============================+
| clang-debug    | Clang debug build          |
+----------------+----------------------------+
| clang-release  | Clang release build        |
+----------------+----------------------------+
| gcc-debug      | GCC debug build            |
+----------------+----------------------------+
| gcc-release    | GCC release build          |
+----------------+----------------------------+
| msvc-debug     | MSVC debug build           |
+----------------+----------------------------+
| msvc-release   | MSVC release build         |
+----------------+----------------------------+

Example with Clang:

.. code-block:: bash

   cmake --preset clang-release -DMAG_NO_DEV_TOOLS=ON
   cmake --build --preset clang-release

To run the test suite for a preset-enabled build:

.. code-block:: bash

   ctest --preset clang-release --output-on-failure

Build Options
-------------

+---------------------------+-----------+------------------------------------------------------------+
| Option                    | Default   | Description                                                |
+===========================+===========+============================================================+
| CMAKE_BUILD_TYPE          | Release   | Standard single-config build type for manual configure     |
+---------------------------+-----------+------------------------------------------------------------+
| BUILD_SHARED_LIBS         | OFF       | Build MAG as a shared library instead of a static library  |
+---------------------------+-----------+------------------------------------------------------------+
| MAG_BUILD_TESTS           | OFF       | Build the Catch2-based test suite                          |
+---------------------------+-----------+------------------------------------------------------------+
| MAG_BUILD_BENCHMARKS      | OFF       | Build benchmark targets                                    |
+---------------------------+-----------+------------------------------------------------------------+
| MAG_ENABLE_SIMD           | ON        | Enable SIMD support on supported architectures             |
+---------------------------+-----------+------------------------------------------------------------+
| MAG_SIMD_BACKEND          | AUTO      | Select SIMD backend: AUTO, SSE2, SSSE3, SSE4_1, or NEON   |
+---------------------------+-----------+------------------------------------------------------------+
| MAG_NO_DEV_TOOLS          | OFF       | Disable development-only helpers and generated tooling     |
+---------------------------+-----------+------------------------------------------------------------+
| MAG_CODE_ANALYSIS         | ON        | Enable clang-tidy on the MAG target when dev tools are on  |
+---------------------------+-----------+------------------------------------------------------------+

Platform notes
--------------

- On x86 and x86_64 targets, MAG enables SSE-family compile options for SIMD builds.
- ``MAG_SIMD_BACKEND`` supports ``AUTO``, ``SSE2``, ``SSSE3``, and ``SSE4_1`` on x86/x64.
- On ARM targets, MAG selects the NEON backend when available.
- ``MAG_SIMD_BACKEND`` supports ``AUTO`` and ``NEON`` on ARM/ARM64.
- The selected backend emits one compile definition:
  ``MAG_SIMD_BACKEND_SSE2``, ``MAG_SIMD_BACKEND_SSSE3``, ``MAG_SIMD_BACKEND_SSE4_1``, or
  ``MAG_SIMD_BACKEND_NEON``.
- On unsupported architectures, MAG disables SIMD backends automatically and falls back to scalar
  implementations in the cmake file.
  - If the project is compiled with SIMD enabled on an unsupported architecture/backend, it will result in a compilation failure.
- Development tooling is only configured for top-level builds when ``MAG_NO_DEV_TOOLS`` is left
  disabled.

Next step
---------

Once MAG builds successfully, continue with :doc:`integration` to link it into another project.
