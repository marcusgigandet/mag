#################################
# Set constants for the project #
#################################
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/mag/*.cpp")
file(GLOB_RECURSE BENCHMARK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*.cpp")

######################
# Add glm to project #
######################
include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	master
)

FetchContent_MakeAvailable(glm)

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
target_compile_options(run_all_tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
target_link_options(run_all_tests PRIVATE -fprofile-instr-generate)

##############################
# Build benchmark executable #
##############################
add_executable(mag_benchmark ${BENCHMARK_SOURCES})
target_link_libraries(mag_benchmark 
        PRIVATE
        mag
        Catch2::Catch2WithMain
        glm::glm
)
