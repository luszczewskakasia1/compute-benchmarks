#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2019-2022 Intel Corporation
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

if (NOT BUILD_SYCL)
    return()
endif()

set(TARGET_NAME show_devices_sycl)
add_executable(${TARGET_NAME} CMakeLists.txt)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER tools)
add_sources_to_benchmark(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE compute_benchmarks_framework_sycl)

# Additional config
setup_vs_folders(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
setup_output_directory(${TARGET_NAME})
