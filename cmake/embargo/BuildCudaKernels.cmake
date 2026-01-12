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

set(KERNELS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/source/kernels")
file(GLOB_RECURSE KERNEL_CU_FILES "${KERNELS_SOURCE_DIR}/*.cu")
if(NOT KERNEL_CU_FILES)
    message(STATUS "No CUDA kernel source files found, skipping CUDA fatbin generation.")
    return()
endif()

if(NOT DEFINED CMAKE_CUDA_COMPILER OR CMAKE_CUDA_COMPILER STREQUAL "" OR NOT EXISTS "${CMAKE_CUDA_COMPILER}")
    message(STATUS "Skipping CUDA fatbin generation: CMAKE_CUDA_COMPILER is not configured or nvcc is unavailable.")
    return()
endif()

file(MAKE_DIRECTORY "${OUTPUT_DIR}")

set(CUDA_ARCH_LIST "${CMAKE_CUDA_ARCHITECTURES}")
if(NOT CUDA_ARCH_LIST)
    set(CUDA_ARCH_LIST "native")
endif()

set(CUDA_USE_NATIVE_ARCH OFF)
list(FIND CUDA_ARCH_LIST "native" CUDA_NATIVE_INDEX)
if(CUDA_NATIVE_INDEX GREATER -1)
    set(CUDA_USE_NATIVE_ARCH ON)
endif()


set(CUDA_ARCH_FLAGS "")
if(CUDA_USE_NATIVE_ARCH)
    list(APPEND CUDA_ARCH_FLAGS "-arch=native")
else()
    foreach(arch ${CUDA_ARCH_LIST})
        if(arch STREQUAL "native")
            continue()
        endif()
        list(APPEND CUDA_ARCH_FLAGS "-gencode=arch=compute_${arch},code=sm_${arch}")
    endforeach()
endif()

set(CUDA_INCLUDE_FLAGS "")
if(CMAKE_SYSTEM_NAME STREQUAL "QNX" AND DEFINED CUDAToolkit_INCLUDE_DIRS)
    foreach(dir ${CUDAToolkit_INCLUDE_DIRS})
        list(APPEND CUDA_INCLUDE_FLAGS "-I${dir}")
    endforeach()
endif()

set(CUDA_FATBIN_OUTPUTS)
foreach(KERNEL_CU_FILE ${KERNEL_CU_FILES})
    get_filename_component(KERNEL_NAME ${KERNEL_CU_FILE} NAME_WE)
    set(FATBIN_OUTPUT "${OUTPUT_DIR}/${KERNEL_NAME}.fatbin")

    add_custom_command(
        OUTPUT ${FATBIN_OUTPUT}
        COMMAND ${CMAKE_CUDA_COMPILER} ${CUDA_INCLUDE_FLAGS}
                ${CUDA_ARCH_FLAGS}
                --fatbin -o ${FATBIN_OUTPUT} ${KERNEL_CU_FILE}
        DEPENDS ${KERNEL_CU_FILE}
        COMMENT "CUDA fatbin: ${KERNEL_NAME}.fatbin"
        VERBATIM
    )

    list(APPEND CUDA_FATBIN_OUTPUTS ${FATBIN_OUTPUT})
endforeach()

add_custom_target(cuda_kernels ALL
    DEPENDS ${CUDA_FATBIN_OUTPUTS}
    COMMENT "Generating CUDA fatbin files"
)

add_dependencies(${TARGET_NAME} cuda_kernels)

if(CUDA_USE_NATIVE_ARCH)
    message(STATUS "CUDA fatbin generation uses: -arch=native (auto-detected architecture).")
else()
    string(REPLACE ";" " " CUDA_GENCODE_SUMMARY "${CUDA_ARCH_FLAGS}")
    message(STATUS "CUDA fatbin generation uses GENCODE flags: ${CUDA_GENCODE_SUMMARY}.")
endif()
