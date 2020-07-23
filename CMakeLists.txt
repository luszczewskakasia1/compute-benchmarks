cmake_minimum_required(VERSION 3.10.0)
project(ULLS_Benchmark)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
set(TARGET_NAME ulls_benchmark)
add_subdirectory(source)
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${TARGET_NAME})
