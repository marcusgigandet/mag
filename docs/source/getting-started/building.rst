Building from Source
====================

Build MAG step-by-step using CMake.

Prerequisites
-------------

Ensure you have installed:

- A modern C++ compiler that supports C++ modules (GCC 15+, Clang 18+, or MSVC <xxx-todo-fix-this-version-number>)
- CMake 3.28.3 or higher
- Ninja

Detailed Build Process
----------------------

Build Options
-------------

+---------------------------+--------------------+-----------+
| Option                    | Values             | Default   |
+===========================+====================+===========+
| CMAKE_BUILD_TYPE          | Debug/Release      | Release   |
+---------------------------+--------------------+-----------+
| BUILD_SHARED_LIBS         | ON/OFF             | ON        |
+---------------------------+--------------------+-----------+
| MAG_BUILD_TESTS           | ON/OFF             | ON        |
+---------------------------+--------------------+-----------+
| MAG_BUILD_BENCHMARKS      | ON/OFF             | ON        |
+---------------------------+--------------------+-----------+
| MAG_ENABLE_SIMD           | ON/OFF             | ON        |
+---------------------------+--------------------+-----------+
| MAG_ENABLE_SIMD_EXTENDED  | ON/OFF             | ON        |
+---------------------------+--------------------+-----------+
| MAG_NO_DEV_TOOLS          | ON/OFF             | ON        |
+---------------------------+--------------------+-----------+
| MAG_CODE_ANALYSIS         | ON/OFF             | ON        |
+---------------------------+--------------------+-----------+
