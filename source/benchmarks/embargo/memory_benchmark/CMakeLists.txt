#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2019-2025 Intel Corporation
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express or
# implied warranties, other than those that are expressly stated in the License.

set(MEMORY_BENCHMARK_SUPPORTED_APIS ocl l0)
if(BUILD_SYCL)
    if(SYCL_FOUND)
        list(APPEND MEMORY_BENCHMARK_SUPPORTED_APIS sycl syclpreview)
    else()
        message(STATUS "SYCL not found thus memory_benchmark_embargo_sycl* will not be built")
    endif()
endif()

add_benchmark(memory_benchmark_embargo ${MEMORY_BENCHMARK_SUPPORTED_APIS})

foreach(API ${MEMORY_BENCHMARK_SUPPORTED_APIS})
    set(TARGET_NAME "memory_benchmark_embargo_${API}")
    set(TARGET_LOCATION "memory_benchmark_embargo_${API}_LOCATION")
    set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/embargo")
    set_property(GLOBAL PROPERTY ${TARGET_LOCATION} "embargo")
endforeach()
