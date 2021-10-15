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

function(add_workload TARGET_NAME API)
    add_executable(${TARGET_NAME} CMakeLists.txt)
    target_link_libraries(${TARGET_NAME} PRIVATE compute_benchmarks_framework_${API})
    target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER workloads)
    if (BUILD_FOR_PUBLISHING)
        set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/workloads/${BASE_TARGET_NAME})
    endif()

    # Add sources
    add_sources_to_benchmark(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
    add_kernels_to_benchmark(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/kernels)
    copy_kernels_to_bin_directory(${TARGET_NAME})
    setup_vs_folders(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
    setup_warning_options(${TARGET_NAME})
endfunction()

add_subdirectory(single_queue_workload_l0)
add_subdirectory(single_queue_workload_shared_buffer_l0)
if (BUILD_HELLO_WORLD)
    add_subdirectory(hello_world_template_workload_ocl)
    add_subdirectory(hello_world_workload_ocl)
endif()
