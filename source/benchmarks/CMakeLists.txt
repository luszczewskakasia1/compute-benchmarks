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

include(CMakeMacrosAddBenchmark.cmake)

set(BENCHMARKS_SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(api_overhead_benchmark)
add_subdirectory(atomic_benchmark)
add_subdirectory(eu_benchmark)
add_subdirectory(gpu_cmds_benchmark)
add_subdirectory(memory_benchmark)
add_subdirectory(miscellaneous_benchmark)
add_subdirectory(multitile_memory_benchmark)
add_subdirectory(overlap_benchmark)
add_subdirectory(ulls_benchmark)
