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

macro(add_custom_benchmark_options)
    benchmark_option(BUILD_CUDA ON)
    benchmark_option(USE_SYSTEM_CUDA ON)

    if (NOT BUILD_CUDA)
        set(BUILD_ALL_API_BINARIES OFF)
        set(BUILD_ALL_API_BINARIES_COMMENT "(Disabled due to not all APIs being enabled)")
    endif()
endmacro()
