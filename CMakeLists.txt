cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)
project(mag LANGUAGES CXX)

include(FetchContent)

#####################
# Compiler settings #
#####################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# We only support clang for now
if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR
            "This project requires Clang. "
            "Detected: ${CMAKE_CXX_COMPILER_ID}"
    )
endif ()

if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 18)
    message(FATAL_ERROR
            "Clang >= 18 is required. "
            "Detected version: ${CMAKE_CXX_COMPILER_VERSION}"
    )
endif ()

###########
# Options #
###########
option(MAG_BUILD_TESTS "Build tests" OFF)
option(MAG_ENABLE_SIMD "Enable simd for supported platforms" ON)
option(MAG_ENABLE_SIMD_EXTENDED "Enable simd for signed/unsigned integers" ON)

#################################
# Set constants for the project #
#################################
file(GLOB_RECURSE MODULES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

if (NOT MAG_ENABLE_SIMD)
    list(FILTER MODULES EXCLUDE REGEX ".*/simd/.*")
else ()
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch64|ARM64")
        set(MAG_PLATFORM_ARM TRUE)
    endif ()

    if (NOT MAG_PLATFORM_ARM)
        list(FILTER MODULES EXCLUDE REGEX ".*/neon/.*")
    endif ()
endif ()

###################
# Target: library #
###################
add_library(${PROJECT_NAME} SHARED)
target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET allModules
        TYPE CXX_MODULES
        FILES ${MODULES}
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

target_compile_features(${PROJECT_NAME}
        PUBLIC
        cxx_std_20
)

if (MAG_ENABLE_SIMD)
    target_compile_definitions(${PROJECT_NAME}
            PRIVATE
            MAG_ENABLE_SIMD
    )
endif ()

###############
# Build tests #
###############
if (MAG_BUILD_TESTS)
    message(STATUS "Building with tests")
    add_compile_definitions(CATCH_CONFIG_NO_FORMAT)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/Catch2)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()

###############################
# Run metadata update targets #
###############################
FetchContent_Declare(
        toolbox
        GIT_REPOSITORY https://github.com/SlyFryFrog/toolbox.git
        GIT_TAG main
)
FetchContent_MakeAvailable(toolbox)

add_custom_target(mag_copyright
        COMMAND "python3"
        "${toolbox_SOURCE_DIR}/scripts/copyright.py"
        --author "Marcus Gigandet"
        --directory "src"
        --fix
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Syncing copyright headers with Git history"
        VERBATIM
)

add_dependencies(${PROJECT_NAME} mag_copyright)

###############################
# Compile commands for clangd #
###############################
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
