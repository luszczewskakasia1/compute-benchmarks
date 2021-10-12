#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2019-2021 Intel Corporation
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express or
# implied warranties, other than those that are expressly stated in the License.
#

if (NOT BUILD_L0)
    return()
endif()

# Get libraries and headers for the SDK
find_package(LevelZero QUIET)
set(LevelZero_FOUND OFF)
if(NOT ${LevelZero_FOUND})
    set(ADDITIONAL_PATH "${CMAKE_SOURCE_DIR}/third_party/level-zero-sdk")
    message(STATUS "Level Zero installation not found. Trying ${ADDITIONAL_PATH}")

    if (WIN32)
        set(LIB_NAME ze_loader.lib)
    else()
        set(LIB_NAME libze_loader.so)
    endif()
    set(LevelZero_LIBRARIES    "${ADDITIONAL_PATH}/lib/x64/${LIB_NAME}")
    set(LevelZero_INCLUDE_DIRS "${ADDITIONAL_PATH}/include")
else()
    message(STATUS "Level Zero installation found")
endif()

# Get sources
file(GLOB_RECURSE SOURCES *.cpp *.h)

# Define target
set(API_NAME l0)
set(TARGET_NAME compute_benchmarks_framework_${API_NAME})
add_library(${TARGET_NAME} STATIC ${SOURCES})
target_link_libraries(${TARGET_NAME} PUBLIC ${LevelZero_LIBRARIES})
target_link_libraries(${TARGET_NAME} PUBLIC compute_benchmarks_framework)
target_include_directories(${TARGET_NAME} PUBLIC ${LevelZero_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/level-zero-intel)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER framework)
add_subdirectories()
setup_vs_folders(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
if (MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Add this API to global array
set_property(GLOBAL APPEND PROPERTY APIS ${API_NAME})
