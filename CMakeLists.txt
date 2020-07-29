cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)
project(ULLS_Benchmark)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Enable MT Compilation in VisualStudio
if (MSVC)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif()

# Option for output dir
if (NOT OUTPUT_DIR)
    set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
endif()
message(STATUS "Binary output directory: ${OUTPUT_DIR}")

# Option for OCL
option(TEST_OCL "Enable OpenCL benchmarks" ON)
if(NOT TEST_OCL)
    message(STATUS "Skipping OpenCL tests")
else()
    message(STATUS "Including OpenCL tests")
endif()

# Option for L0
option(TEST_L0 "Enable LevelZero benchmarks" ON)
if(NOT TEST_L0)
    message(STATUS "Skipping LevelZero tests")
else()
    message(STATUS "Including LevelZero tests")
endif()

# Load dependencies
if(NOT EXISTS third_party/googletest/CMakeLists.txt)
    execute_process(COMMAND git submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

# Gtest
set(gtest_force_shared_crt OFF CACHE BOOL "")
set(BUILD_GMOCK OFF CACHE BOOL "")
add_subdirectory(third_party/googletest)

# Benchmark
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_DEBUG} /MT")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_DEBUG} /MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
add_subdirectory(source)
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ulls_benchmark)
