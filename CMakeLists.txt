# Configuration

cmake_minimum_required(VERSION 3.10)

project(noisette LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

# noisette

add_library(noisette INTERFACE)
target_include_directories(noisette INTERFACE include)

# Static analysis

find_program(CMAKE_CXX_CLANG_TIDY NAMES clang-tidy)
if(CMAKE_CXX_CLANG_TIDY)
    string(CONCAT CLANG_TIDY_ARGS
        "bugprone-*,"
        "cppcoreguidelines-*,"
        "modernize-*,"
        "performance-*,"
        "portability-*,"
        "readability-*,"
        "-readability-braces-around-statements,"
        "-readability-else-after-return,"
        "-clang-diagnostic-unknown-warning-option")
    list(APPEND CMAKE_CXX_CLANG_TIDY "-checks=${CLANG_TIDY_ARGS}")
endif()

# Examples

add_subdirectory(examples)