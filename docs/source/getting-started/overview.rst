Library Overview
================

MAG is a C++20 math library for graphics-related development such as rendering engines.

The public API is exposed through a single module shown below:

.. code-block:: c++

   import mag;

Common types
------------

You will typically work with aliases such as the following exposed in the ``mag`` namespace and the ``mag`` module:

- ``Vec2f``, ``Vec3f``, ``Vec4f``
- ``Mat3f``, ``Mat4f``

All simd-related aliases/types are exposed in the ``mag::simd`` namespace and the ``mag.simd`` module.

Build requirements
------------------

MAG depends on a compiler with usable C++20 module support. The top-level build currently checks
for these minimum versions:

+------------+-------------------------------------------+
| Tool       | Minimum version                           |
+============+===========================================+
| Clang      | 18                                        |
+------------+-------------------------------------------+
| GCC        | 15                                        |
+------------+-------------------------------------------+
| MSVC       | Visual Studio 2022 17.10 / MSVC 19.40     |
+------------+-------------------------------------------+
| CMake      | 3.28.3                                    |
+------------+-------------------------------------------+
| Ninja      | Recommended generator used by presets     |
+------------+-------------------------------------------+

For a full setup walkthrough, continue with :doc:`building`.
