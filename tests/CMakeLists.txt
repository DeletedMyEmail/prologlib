cmake_minimum_required(VERSION 3.14)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Find all test source files in the current directory
file(GLOB TEST_SOURCES "*.cpp")

# Add the test executable
add_executable(run_tests ${TEST_SOURCES})

# Link the test executable against your library and GoogleTest
# The PROJECT_NAME variable is inherited from the top-level CMakeLists.txt
target_link_libraries(run_tests PRIVATE ${PROJECT_NAME} GTest::gtest_main)

# Enable test discovery
include(GoogleTest)
gtest_discover_tests(run_tests)