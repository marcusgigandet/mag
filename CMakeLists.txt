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

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 18)
        message(FATAL_ERROR
                "Clang >= 18 is required. "
                "Detected version: ${CMAKE_CXX_COMPILER_VERSION}"
        )
    endif ()
endif ()

###########
# Options #
###########
option(MAG_BUILD_TESTS "Build tests" OFF)
option(MAG_ENABLE_SIMD "Enable simd for supported platforms" ON)
option(MAG_ENABLE_SIMD_EXTENDED "Enable simd for signed/unsigned integers" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

#################
# Set constants #
#################
file(GLOB_RECURSE MODULES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Remove unused code
if (NOT MAG_ENABLE_SIMD)
    list(FILTER MODULES EXCLUDE REGEX ".*/simd/.*")
else ()
    # Filter based on x86 and arm
    # Add more architectures if we add future support for something like RISC-V
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch64|ARM64")
        set(MAG_PLATFORM_ARM TRUE)
        list(FILTER MODULES EXCLUDE REGEX ".*/x86/.*")
    else()
        list(FILTER MODULES EXCLUDE REGEX ".*/neon/.*")
        add_compile_options(-msse2 -msse3)
    endif ()
endif ()
###################
# Target: library #
###################
add_library(mag)
add_library(mag::mag ALIAS mag)

target_sources(mag
        PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES ${MODULES}
)

target_include_directories(mag
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

target_compile_features(mag
        PUBLIC
        cxx_std_20
)

if (MAG_ENABLE_SIMD)
    target_compile_definitions(mag PRIVATE MAG_ENABLE_SIMD)
endif ()

#####################
# Build-tree export #
#####################
export(TARGETS mag FILE "${CMAKE_CURRENT_BINARY_DIR}/mag_targets.cmake")

###############
# Build tests #
###############
if (MAG_BUILD_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/Catch2)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()

#############
# Dev tools #
#############
if (PROJECT_IS_TOP_LEVEL)
    add_custom_target(mag_copyright
            COMMAND python3
            "${CMAKE_CURRENT_SOURCE_DIR}/vendor/toolbox/scripts/copyright/copyright.py"
            --config "${CMAKE_CURRENT_SOURCE_DIR}/config.toml"
            --directory "${CMAKE_CURRENT_SOURCE_DIR}/src"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Syncing copyright headers with Git history"
            VERBATIM
    )

    add_dependencies(mag mag_copyright)
endif ()

#########################
# Compile commands copy #
#########################
if (PROJECT_IS_TOP_LEVEL)
add_custom_command(TARGET mag POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
endif ()

###################
# Install targets #
###################
install(TARGETS mag
        EXPORT mag_targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        FILE_SET cxx_modules DESTINATION modules
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/
        DESTINATION modules
        FILES_MATCHING PATTERN "*.cppm"
)
