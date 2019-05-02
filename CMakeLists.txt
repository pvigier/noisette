# Configuration

cmake_minimum_required(VERSION 3.8.2)

project(noisette LANGUAGES CXX VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

# noisette

add_library(noisette INTERFACE)
target_include_directories(noisette INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>)

# Examples

option (BUILD_EXAMPLES "Build the examples." ON)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Install

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/noisetteConfigVersion.cmake
    VERSION 
    COMPATIBILITY AnyNewerVersion)
configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/noisetteConfig.cmake.in
    ${CMAKE_BINARY_DIR}/noisetteConfig.cmake
    INSTALL_DESTINATION lib/cmake/noisette)

install(TARGETS noisette EXPORT noisetteTargets
    INCLUDES DESTINATION include)
install(EXPORT noisetteTargets
    FILE noisetteTargets.cmake
    DESTINATION lib/cmake/noisette)
install(FILES 
    ${CMAKE_BINARY_DIR}/noisetteConfigVersion.cmake
    ${CMAKE_BINARY_DIR}/noisetteConfig.cmake
    DESTINATION lib/cmake/noisette)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)