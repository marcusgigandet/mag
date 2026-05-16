#################################
# Set constants for the project #
#################################
file(GLOB_RECURSE BENCHMARK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/mag/*.cpp")

######################
# Add glm to project #
######################
include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG master
)

FetchContent_MakeAvailable(glm)

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
