cmake_minimum_required(VERSION 4.1)
project(log LANGUAGES CXX)

include(cmake/DetriDependencies.cmake)

option(DETRI_LOG_BUILD_TESTS "Whether to build logging tests" ${PROJECT_IS_TOP_LEVEL})

add_library(detri_log STATIC)
add_library(detri::log ALIAS detri_log)

set_target_properties(detri_log PROPERTIES
    CXX_STANDARD 26
    CXX_EXTENSIONS OFF
)

target_sources(detri_log
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS src
        FILES src/detri/log.hpp
    PRIVATE
        src/detri/log.cpp
)

detri_resolve_dependency(
    TARGET fmt::fmt
    PACKAGE fmt
    FETCHCONTENT_NAME fmt
    FETCHCONTENT_DECLARE_ARGS
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 12.1.0
)

if (NOT TARGET spdlog::spdlog)
    set(SPDLOG_USE_EXTERNAL_FMT ON CACHE BOOL "" FORCE)
endif()

detri_resolve_dependency(
    TARGET spdlog::spdlog
    PACKAGE spdlog
    FETCHCONTENT_NAME spdlog
    FETCHCONTENT_DECLARE_ARGS
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.17.0
)

target_link_libraries(detri_log
    PUBLIC fmt::fmt
    PRIVATE spdlog::spdlog
)

if (PROJECT_IS_TOP_LEVEL AND DETRI_LOG_BUILD_TESTS)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    detri_resolve_dependency(
        TARGET GTest::gtest_main
        PACKAGE GTest
        FETCHCONTENT_NAME googletest
        FETCHCONTENT_DECLARE_ARGS
            GIT_REPOSITORY https://github.com/google/googletest
            GIT_TAG v1.17.0
    )

    add_executable(log_test src/test/spdlog_logger_test.cpp)
    target_link_libraries(log_test PRIVATE detri_log spdlog::spdlog GTest::gtest_main)

    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(log_test)
endif()