Using MAG
=========

MAG is designed around a single module import:

.. code-block:: c++

   import mag;
   using namespace mag;

The examples below show the main workflows exposed by the public API.

Vector math
-----------

.. code-block:: c++

   import mag;
   using namespace mag;

   Vec4f a{1.0f, 2.0f, 3.0f, 4.0f};
   Vec4f b{3.0f, 4.0f, 5.0f, 6.0f};

   auto sum = a + b;
   auto scaled = 0.5f * a;
   auto multiplied = a * b;
   float len = a.length();
   float dotValue = a.dot(b);
   auto normalized = a.normalized();
   auto lerped = lerp(a, b, 0.25f);

Vectors support component aliases where they make sense. For example, 4D vectors expose both
``x/y/z/w`` and ``r/g/b/a`` accessors.

Matrices also support component aliases for specific implementations such as 3x3 and 4x4 matrices.

Matrix math
-----------

Matrices are structured into column major 2D arrays meaning that data is stored into columns, rows in the array.

.. code-block:: c++

   import mag;
   using namespace mag;

   Mat4f model{Mat4f::translate(1.0f, 2.0f, 3.0f) * Mat4f::rotateZ(pi<float> / 4.0f)};

   Vec4f point{1.0f, 0.0f, 0.0f, 1.0f};
   Vec4f transformed{model * point};

   Mat4f inverse{model.inverse()};
   Mat4f identity{model * inverse};
   Mat4f scale{Mat4f::diagonal(Vec4f{2.0f, 3.0f, 4.0f, 1.0f})};

SIMD helpers
------------

When ``MAG_ENABLE_SIMD`` is enabled and the target architecture supports it, the ``mag`` module also
exports SIMD functionality in a separate namespace, ``mag::simd``:

.. code-block:: c++

   #include <array>

   import mag;
   using namespace mag::simd;

   std::array<float, 4> data{5.0f, 6.0f, 7.0f, 8.0f};

   f32x4 a{2.0f, -4.0f, 8.0f, -16.0f};
   f32x4 b{1.0f, 2.0f, -4.0f, -8.0f};

   auto mixed{(a + b) * 0.5f};
   auto scalarLeft{2.0f - b};
   auto fromPointer{load<float, fixed_abi<4>>(data.data())};
   auto splatted = splat<float, fixed_abi<4>>(9.0f);

   float total{hsum(a)};
   float minValue{hmin(a)};
   float maxValue{hmax(a)};

   std::array<float, 4> out{};
   mixed.store(out.data());

The SIMD API supports arithmetic, reductions, load/store helpers, fixed-width ABI types, and native ABI aliases where available.

**Note**: The SIMD API is subject to changes as it is still a WIP.