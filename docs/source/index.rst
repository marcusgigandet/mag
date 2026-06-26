Welcome to MAG
==============

**MAG (Mathematics and Graphics)** is a modern-cpp math library built for use in 3D graphics.
The library targets **C++20 and newer**, uses **C++ modules**, and ships a compact API for
vector math, matrix math, transform helpers, constants, and optional SIMD acceleration.

What you get
------------

- A single public module import: ``import mag;``
- Vector types for 2D, 3D, and 4D math
- Matrix types and transform helpers for graphics workflows
- Optional SIMD types and helpers when SIMD support is enabled at build time
- CMake integration for both in-tree and dependency-based builds

Start here
----------

- :doc:`getting-started/overview` - Library overview and module layout
- :doc:`getting-started/building` - Configure, build, and test MAG
- :doc:`getting-started/integration` - Add MAG to another CMake project
- :doc:`getting-started/using-mag` - Basic vector, matrix, and SIMD examples

.. toctree::
   :hidden:

   getting-started/overview
   getting-started/building
   getting-started/integration
   getting-started/using-mag
