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

# Define Framework library
file(GLOB SOURCES *.cpp *.h)
set(TARGET_NAME compute_benchmarks_framework)
add_library(${TARGET_NAME} STATIC ${SOURCES})
target_link_libraries(${TARGET_NAME} PUBLIC gtest)
target_include_directories(${TARGET_NAME} PUBLIC ${SOURCE_ROOT})
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER framework)
add_subdirectories()

# Ignore CRT Warnings
if (MSVC)
    target_compile_definitions(${TARGET_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

# Export all symbols on VS builds (so __dllexport is not required)
if (MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Create directory structure in Visual Studio
setup_vs_folders(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
