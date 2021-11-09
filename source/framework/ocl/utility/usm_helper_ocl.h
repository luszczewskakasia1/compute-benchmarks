/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#pragma once

#include "framework/enum/device_selection.h"
#include "framework/enum/usm_initial_placement.h"
#include "framework/enum/usm_memory_placement.h"
#include "framework/ocl/opencl.h"

struct UsmHelperOcl {
    struct Alloc {
        void *ptr = nullptr;
        UsmMemoryPlacement placement = UsmMemoryPlacement::Unknown;
        UsmFunctions usm = {};
        cl_context context = {};

        struct MappedData {
            cl_mem memObject = {};
            cl_command_queue queue = {};
        } mappedData;
    };

    static cl_int allocate(Opencl &opencl,
                           UsmMemoryPlacement placement,
                           size_t bufferSize,
                           Alloc &outAlloc);
    static cl_int deallocate(Alloc &alloc);

    static void *allocate(DeviceSelection placement,
                          Opencl &opencl,
                          size_t bufferSize,
                          cl_int *retVal);

    static cl_mem_properties_intel getInitialPlacementFlag(UsmInitialPlacement placement);
};
