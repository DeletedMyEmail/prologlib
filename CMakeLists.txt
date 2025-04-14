cmake_minimum_required(VERSION 3.28)
project(CppCustomLib LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}")

include(FetchContent)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(spdlog)

file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")
include_directories("include/")
add_library(${PROJECT_NAME} ${CPP_SOURCES})

target_link_libraries(${PROJECT_NAME} spdlog::spdlog)