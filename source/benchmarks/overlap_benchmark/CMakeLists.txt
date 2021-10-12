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

add_benchmark(overlap_benchmark ocl l0 all)
add_benchmark_dependency_on_workload(overlap_benchmark single_queue_workload_l0 l0)
add_benchmark_dependency_on_workload(overlap_benchmark single_queue_workload_shared_buffer_l0 l0)
