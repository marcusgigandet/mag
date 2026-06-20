#################################
# Set constants for the project #
#################################
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/mag/*.cpp")

# Exclude simd tests if they weren't enabled
if (NOT MAG_ENABLE_SIMD)
    list(FILTER TEST_SOURCES EXCLUDE REGEX ".*/simd/.*")
endif ()

###############
# Build tests #
###############
message(STATUS "Found test sources: ${TEST_SOURCES}")
foreach (src ${TEST_SOURCES})
    get_filename_component(name "${src}" NAME_WE)

    add_executable(${name} ${src})
    target_link_libraries(${name}
            PRIVATE
            mag
            Catch2::Catch2WithMain
    )

    add_test(NAME ${name} COMMAND ${name})
endforeach ()

########################################
# Build all tests as single executable #
########################################
add_executable(run_all_tests ${TEST_SOURCES})
target_link_libraries(run_all_tests
        PRIVATE
        mag
        Catch2::Catch2WithMain
)

add_test(NAME mag_all_tests COMMAND run_all_tests)
