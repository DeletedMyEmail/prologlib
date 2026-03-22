cmake_minimum_required(VERSION 3.14)
project(prologlib LANGUAGES CXX VERSION 1.0.1 DESCRIPTION "Library for logging and performance profiling")
option(BUILD_TESTING "Build the tests for this project" ON)
include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 23)
add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}")

include(FetchContent)
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.17.0)
FetchContent_MakeAvailable(spdlog)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

file(GLOB CPP_SRC "src/*.cpp")
file(GLOB HEADER_SRC "include/*.h")

add_library(${PROJECT_NAME} ${CPP_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        PUBLIC_HEADER "${HEADER_SRC}")

configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
target_include_directories(${PROJECT_NAME} PUBLIC "include/")
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()