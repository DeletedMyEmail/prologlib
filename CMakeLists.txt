cmake_minimum_required(VERSION 3.14)
project(cstmlib LANGUAGES CXX VERSION 0.1.0 DESCRIPTION "Library for logging and performance profiling")
include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 20)
add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}")

include(FetchContent)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(spdlog)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

file(GLOB CPP_SRC "src/*.cpp")
file(GLOB CPP_SRC "include/*.h")

add_library(${PROJECT_NAME} ${CPP_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        PUBLIC_HEADER ${HEADERS_SRC})

configure_file(mylib.pc.in mylib.pc @ONLY)
target_include_directories(${PROJECT_NAME} PRIVATE "include/")
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

install(TARGETS ${PROJECT_NAME}}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_BINARY_DIR}/mylib.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
