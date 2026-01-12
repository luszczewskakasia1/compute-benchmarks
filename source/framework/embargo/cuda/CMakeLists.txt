#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2025-2026 Intel Corporation
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express or
# implied warranties, other than those that are expressly stated in the License.

if(NOT BUILD_CUDA)
    return()
endif()

# Get libraries and headers for the SDK
if (USE_SYSTEM_CUDA)
    find_package(CUDAToolkit QUIET)
endif()

include(CheckLanguage)
check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
endif()

if (NOT CUDAToolkit_FOUND)
    message(STATUS "Using CUDA from SDK sources")

    if (WIN32)
        set(CUDART_LIB_NAME "cudart.lib")
        set(CUDA_DRIVER_LIB_NAME "cuda.lib")
    else()
        set(CUDART_LIB_NAME "libcudart.so")
        set(CUDA_DRIVER_LIB_NAME "libcuda.so")
    endif()
    
    set(CUDAToolkit_LIBRARIES
        "${CMAKE_SOURCE_DIR}/third_party/embargo/cuda-sdk/lib/x64/${CUDART_LIB_NAME}"
        "${CMAKE_SOURCE_DIR}/third_party/embargo/cuda-sdk/lib/x64/${CUDA_DRIVER_LIB_NAME}"
    )
    set(CUDAToolkit_INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/third_party/embargo/cuda-sdk/include/cuda"
    )
else()
    message(STATUS "Using system CUDAToolkit found in: ${CUDAToolkit_LIBRARY_DIR}")
endif()

# Get sources
file(GLOB_RECURSE SOURCES *.cpp *.h)

# Define target
set(API_NAME cuda)
set(TARGET_NAME compute_benchmarks_framework_${API_NAME})

add_library(${TARGET_NAME} STATIC ${SOURCES})
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER framework)

if (CUDAToolkit_FOUND)
  target_link_libraries(${TARGET_NAME} PUBLIC CUDA::cudart CUDA::cuda_driver)
else()
  target_link_libraries(${TARGET_NAME} PUBLIC ${CUDAToolkit_LIBRARIES})
  set_target_properties(${TARGET_NAME} PROPERTIES BUILD_RPATH "${CUDA_LIB_DIR}")
endif()

target_link_libraries(${TARGET_NAME} PUBLIC compute_benchmarks_framework)
target_include_directories(${TARGET_NAME} PUBLIC ${CUDAToolkit_INCLUDE_DIRS})

add_subdirectories()
setup_vs_folders(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
setup_warning_options(${TARGET_NAME})
setup_output_directory(${TARGET_NAME})

set(CMAKE_CUDA_ARCHITECTURES native)
include(${CMAKE_MODULE_PATH}/embargo/BuildCudaKernels.cmake)

set_property(GLOBAL APPEND PROPERTY APIS ${API_NAME})
