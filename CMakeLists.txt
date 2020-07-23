cmake_minimum_required(VERSION 3.13.0)
project(ULLS_Benchmark)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Gtest
set(gtest_force_shared_crt off)
set(BUILD_GMOCK off)
add_subdirectory(third_party/googletest)

# Benchmark
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
set(TARGET_NAME ulls_benchmark)
add_subdirectory(source)
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${TARGET_NAME})
