cmake_minimum_required(VERSION 4.1)
project(detri_log)

set(CMAKE_CXX_STANDARD 26)

include(FetchContent)

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

add_library(detri_log STATIC)
target_sources(detri_log PUBLIC FILE_SET HEADERS
    BASE_DIRS src
    FILES src/detri/log.hpp
)
target_sources(detri_log PRIVATE src/detri/log.cpp)
target_link_libraries(detri_log PUBLIC fmt::fmt PRIVATE spdlog::spdlog)

add_library(detri::log ALIAS detri_log)
