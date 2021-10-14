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

#include "framework/enum/hostptr_reuse_mode.h"
#include "framework/ocl/function_signatures_ocl.h"
#include "framework/ocl/opencl.h"
#include "framework/test_case/test_result.h"

struct HostptrReuseHelper {
    struct Alloc {
        void *ptr = nullptr;

        HostptrReuseMode reuseMode = HostptrReuseMode::Unknown;
        cl_mem memObject = nullptr;
        cl_command_queue queue = nullptr;
        cl_context context = nullptr;
        pfn_clMemFreeINTEL clMemFreeINTEL = nullptr;
    };

    static cl_int allocateBufferHostptr(Opencl &opencl,
                                        HostptrReuseMode reuseMode,
                                        size_t size,
                                        Alloc &outAlloc);
    static cl_int deallocateBufferHostptr(Alloc alloc);
};
