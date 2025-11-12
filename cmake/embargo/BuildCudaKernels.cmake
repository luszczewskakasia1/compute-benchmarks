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

set(KERNELS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/source/kernels")

file(GLOB_RECURSE KERNEL_CU_FILES
    "${KERNELS_SOURCE_DIR}/*.cu"
)

if(NOT KERNEL_CU_FILES)
    return()
endif()

file(MAKE_DIRECTORY "${OUTPUT_DIR}")

set(GENCODE_FLAGS "")
foreach(arch ${CMAKE_CUDA_ARCHITECTURES})
    list(APPEND GENCODE_FLAGS "-gencode=arch=compute_${arch},code=sm_${arch}")
endforeach()

if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    foreach(dir ${CUDAToolkit_INCLUDE_DIRS})
        list(APPEND INCLUDES "-I${dir}")
    endforeach()
endif()

set(ALL_FATBIN_OUTPUTS)

foreach(KERNEL_CU_FILE ${KERNEL_CU_FILES})
    get_filename_component(KERNEL_NAME ${KERNEL_CU_FILE} NAME_WE)
    set(FATBIN_OUTPUT "${OUTPUT_DIR}/${KERNEL_NAME}.fatbin")

    foreach(KERNEL_FILE ${KERNEL_CU_FILE})
        configure_file("${KERNEL_FILE}" "${OUTPUT_DIR}" COPYONLY)
    endforeach()
    
    add_custom_command(
        OUTPUT ${FATBIN_OUTPUT}
        COMMAND ${CMAKE_CUDA_COMPILER} ${INCLUDES} ${ALL_CCFLAGS} -Wno-deprecated-gpu-targets  ${GENCODE_FLAGS} -o ${FATBIN_OUTPUT} -fatbin ${KERNEL_CU_FILE}
        DEPENDS ${KERNEL_CU_FILE}
        COMMENT "Building CUDA fatbin: ${KERNEL_NAME}.fatbin"
        VERBATIM
    )
    
    list(APPEND ALL_FATBIN_OUTPUTS ${FATBIN_OUTPUT})
endforeach()

add_custom_target(generate_all_fatbins ALL
    DEPENDS ${ALL_FATBIN_OUTPUTS}
    COMMENT "Generating all CUDA fatbin files"
)

add_custom_target(cuda_kernels ALL
    DEPENDS ${ALL_FATBIN_OUTPUTS}
    COMMENT "Generating all CUDA fatbin files"
)
