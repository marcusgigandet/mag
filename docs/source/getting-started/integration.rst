Integrating MAG
===============

MAG is built with CMake and exposes the ``mag::mag`` target. The easiest integration paths are
either ``FetchContent`` or ``add_subdirectory``.

Using FetchContent
------------------

.. code-block:: cmake

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

**Note**: Using FetchContent will also fetch all subdirectories by default.

Using add_subdirectory
----------------------

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.28.3)
   project(my_app LANGUAGES CXX)

   add_subdirectory(vendor/mag)

   add_executable(my_app main.cpp)
   target_link_libraries(my_app PRIVATE mag::mag)
   target_compile_features(my_app PRIVATE cxx_std_20)
