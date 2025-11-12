#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2025 Intel Corporation
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express or
# implied warranties, other than those that are expressly stated in the License.

if(NOT BUILD_OPT)
    return()
endif()

set(CUDA_FOUND FALSE CACHE BOOL "" FORCE)

include(CheckLanguage)
check_language(CUDA)
if(NOT CMAKE_CUDA_COMPILER)
    message(WARNING "Missing CUDA compiler. OPT backend will not be built.")
    return()
endif()
enable_language(CUDA)

find_package(CUDAToolkit REQUIRED)
if(NOT CUDAToolkit_FOUND)
    message(WARNING "Missing CUDA Toolkit. OPT backend will not be built.")
    return()
endif()

# Get sources
include(${CMAKE_MODULE_PATH}/embargo/BuildCudaKernels.cmake)

file(GLOB_RECURSE SOURCES *.cpp *.h)

# Define target
set(API_NAME cuda)
set(TARGET_NAME compute_benchmarks_framework_${API_NAME})
add_library(${TARGET_NAME} STATIC ${SOURCES})
target_compile_options(${TARGET_NAME} INTERFACE $<$<COMPILE_LANGUAGE:CUDA>:--extended-lambda>)
set(CMAKE_CUDA_ARCHITECTURES 75 80 86 87 89 90 100 110 120 CACHE STRING "CUDA architectures")
target_link_libraries(${TARGET_NAME} PUBLIC CUDA::cudart CUDA::cuda_driver)
target_link_libraries(${TARGET_NAME} PUBLIC compute_benchmarks_framework)
target_include_directories(${TARGET_NAME} PUBLIC ${CUDAToolkit_INCLUDE_DIRS})
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER framework)
add_subdirectories()
setup_vs_folders(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
setup_warning_options(${TARGET_NAME})
setup_output_directory(${TARGET_NAME})

set(CUDA_FOUND TRUE CACHE BOOL "" FORCE)
set_property(GLOBAL APPEND PROPERTY APIS ${API_NAME})
