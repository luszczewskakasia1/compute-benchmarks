cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)
project(ComputeBenchmarks)
include(CMakeOptions.cmake)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")



# Enable MT Compilation in VisualStudio
if (MSVC)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif()

# Load dependencies
if(NOT EXISTS third_party/googletest/CMakeLists.txt)
    execute_process(COMMAND git submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

# Publish build additional handling
if(BUILD_FOR_PUBLISHING)
    # Only publish release builds
    message(STATUS "Overriding build type to Release")
    set(CMAKE_CONFIGURATION_TYPES Release)
    set(CMAKE_BUILD_TYPE Release)

    # Produce version string
    string(TIMESTAMP versionDate "%Y%m%d")
    set(versionIndex "01")
    execute_process (COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE versionCommit
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(version "${versionDate}_${versionIndex}_${versionCommit}")
    file(WRITE ${OUTPUT_DIR}/version.txt ${version})
    message(STATUS "Version ${version} written to version.txt")
endif()

# Gtest
set(gtest_force_shared_crt OFF CACHE BOOL "")
set(BUILD_GMOCK OFF CACHE BOOL "")
add_subdirectory(third_party/googletest)
if (MSVC)
    set_target_properties(gtest PROPERTIES FOLDER gtest)
    set_target_properties(gtest_main PROPERTIES FOLDER gtest)
endif()

# Benchmark
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_DEBUG} /MT")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_DEBUG} /MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
add_subdirectory(source)
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ulls_benchmark)
