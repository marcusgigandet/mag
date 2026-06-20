cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)
project(mag VERSION 1.0.0 LANGUAGES CXX)

#####################
# Compiler settings #
#####################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (PROJECT_IS_TOP_LEVEL)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 18)
        message(FATAL_ERROR
                "Clang >= 18 is required. "
                "Detected version: ${CMAKE_CXX_COMPILER_VERSION}"
        )
    endif ()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15)
        message(FATAL_ERROR
                "GCC >= 15 is required. "
                "Detected version: ${CMAKE_CXX_COMPILER_VERSION}"
        )
    endif ()
elseif (MSVC)
    if (MSVC_VERSION LESS 1940) # I think that this is the minimum required?
        message(FATAL_ERROR
                "MSVC >= 19.40 (Visual Studio 2022 17.10) is required. "
                "Detected version: ${MSVC_VERSION}"
        )
    endif ()
else ()
    message("WARNING: Using unsupported compiler - functionality may not work as expected.")
endif ()

###########
# Options #
###########
option(MAG_BUILD_TESTS "Build tests" OFF)
option(MAG_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(MAG_ENABLE_SIMD "Enable simd for supported platforms" ON)
option(MAG_ENABLE_SIMD_EXTENDED "Enable simd for signed/unsigned integers" ON)
option(MAG_NO_DEV_TOOLS "Disable all non-essential dev tools" OFF)
option(MAG_CODE_ANALYSIS "Analyze code with clang-tidy" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

#################
# Set constants #
#################
file(GLOB_RECURSE MODULES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)

set(MAG_NEON_SIMD FALSE CACHE BOOL "" FORCE)
set(MAG_X86_SIMD FALSE CACHE BOOL "" FORCE)
set(MAG_SIMD_COMPILE_OPTIONS "")

# Remove unused code
if (NOT MAG_ENABLE_SIMD)
    list(FILTER MODULES EXCLUDE REGEX ".*/simd/.*")
else ()
    # Make arch consistent across platforms
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH)

    # Filter based on x86 and arm
    # Add more architectures if we add future support for something like RISC-V
    if (ARCH MATCHES "^(aarch64|arm64|arm|armv.*)")
        set(MAG_NEON_SIMD TRUE CACHE BOOL "" FORCE)

        list(FILTER MODULES EXCLUDE REGEX ".*/x86_64/.*")
        list(FILTER MODULES EXCLUDE REGEX ".*/x86/.*")

    elseif (ARCH MATCHES "^(x86_64|amd64|i[3-6]86|x64)")
        set(MAG_X86_SIMD TRUE CACHE BOOL "" FORCE)

        list(FILTER MODULES EXCLUDE REGEX ".*/neon/.*")
        add_compile_options(-msse2 -msse3 -msse4.1)
    else ()
        list(FILTER MODULES EXCLUDE REGEX ".*/simd/.*")
        set(MAG_ENABLE_SIMD OFF)
        message(WARNING
                "SIMD backends are not configured for processor '${CMAKE_SYSTEM_PROCESSOR}'. "
                "MAG will fall back to scalar implementations."
        )
    endif ()
endif ()

###################
# Target: library #
###################
add_library(mag ${HEADERS})
add_library(mag::mag ALIAS mag)

target_sources(mag
        PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES ${MODULES}
)

target_include_directories(mag
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

target_compile_features(mag
        PUBLIC
        cxx_std_20
)

target_compile_options(mag
        PRIVATE
        ${MAG_SIMD_COMPILE_OPTIONS}
)

#######################################
# Add compile definitions and options #
#######################################
target_compile_definitions(mag PRIVATE
        $<$<BOOL:${MAG_ENABLE_SIMD}>:MAG_ENABLE_SIMD>
        $<$<BOOL:${MAG_ENABLE_SIMD_EXTENDED}>:MAG_ENABLE_SIMD_EXTENDED>
        $<$<BOOL:${MAG_NEON_SIMD}>:MAG_NEON_SIMD>
        $<$<BOOL:${MAG_X86_SIMD}>:MAG_X86_SIMD>
)

#####################
# Build-tree export #
#####################
export(TARGETS mag FILE "${CMAKE_CURRENT_BINARY_DIR}/mag_targets.cmake")

###############
# Build tests #
###############
if (MAG_BUILD_TESTS OR MAG_BUILD_BENCHMARKS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/Catch2)
endif ()

if (MAG_BUILD_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()

####################
# Build benchmarks #
####################
if (MAG_BUILD_BENCHMARKS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/benchmarks)
endif ()

#############
# Dev tools #
#############
if (PROJECT_IS_TOP_LEVEL AND NOT ${MAG_NO_DEV_TOOLS})
    include(${CMAKE_CURRENT_SOURCE_DIR}/vendor/toolbox/cmake/CodeQuality.cmake)

    file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    file(GLOB_RECURSE BENCHMARK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*.cpp")

    enforce_clang_format(
            TARGET ${PROJECT_NAME}
            FILES ${MODULES} ${TEST_SOURCES} ${BENCHMARK_SOURCES} ${HEADERS}
    )
    enforce_copyright(
            TARGET ${PROJECT_NAME}
            CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/config.toml"
            SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )
    generate_clangd_compdb(
            TARGET ${PROJECT_NAME}
            DEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    if (MAG_CODE_ANALYSIS)
        set_target_properties(mag PROPERTIES
                CXX_CLANG_TIDY
                "clang-tidy;
            -p=${CMAKE_BINARY_DIR};
            --system-headers=0;
            -header-filter=^${CMAKE_SOURCE_DIR}/src/.*"
        )
    endif ()
endif ()
