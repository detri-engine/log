cmake_minimum_required(VERSION 4.1)
project(log)

set(CMAKE_CXX_STANDARD 26)

option(DETRI_LOG_BUILD_TESTS "Whether to build logging tests" ON)

include(FetchContent)

add_library(detri_log STATIC)
add_library(detri::log ALIAS detri_log)


target_sources(detri_log PUBLIC FILE_SET HEADERS
    BASE_DIRS src
    FILES src/detri/log.hpp
)
target_sources(detri_log PRIVATE src/detri/log.cpp)

# Link external libs
FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 12.1.0
)
FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.17.0
)

set(SPDLOG_USE_EXTERNAL_FMT ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(fmt spdlog)
target_link_libraries(detri_log PUBLIC fmt::fmt PRIVATE spdlog::spdlog)


if (DETRI_LOG_BUILD_TESTS)
    FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest
            GIT_TAG v1.17.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(log_test src/test/spdlog_logger_test.cpp)
    target_link_libraries(log_test PRIVATE detri_log spdlog::spdlog GTest::gtest_main)

    enable_testing()

    include(GoogleTest)
    gtest_discover_tests(log_test)
endif()
