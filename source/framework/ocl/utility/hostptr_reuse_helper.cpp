/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021-2022 Intel Corporation
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

#include "framework/ocl/utility/hostptr_reuse_helper.h"

#include "framework/ocl/utility/error.h"

cl_int HostptrReuseHelper::allocateBufferHostptr(Opencl &opencl,
                                                 HostptrReuseMode reuseMode,
                                                 size_t size,
                                                 Alloc &outAlloc) {
    outAlloc.reuseMode = reuseMode;
    outAlloc.queue = opencl.commandQueue;
    outAlloc.context = opencl.context;

    cl_int retVal{};
    switch (reuseMode) {
    case HostptrReuseMode::None:
        outAlloc.ptr = new uint8_t[size];
        break;
    case HostptrReuseMode::Usm: {
        const auto usmFunctions = opencl.getExtensions().queryUsmFunctions();
        if (!opencl.getExtensions().areUsmPointersNotNull(usmFunctions)) {
            FATAL_ERROR("USM extension not found");
        }
        outAlloc.ptr = usmFunctions.clHostMemAllocINTEL(outAlloc.context, nullptr, size, 0llu, &retVal);
        outAlloc.clMemFreeINTEL = usmFunctions.clMemFreeINTEL;
        break;
    }
    case HostptrReuseMode::Map:
        outAlloc.memObject = clCreateBuffer(outAlloc.context, CL_MEM_READ_WRITE, size, nullptr, &retVal);
        CL_SUCCESS_OR_RETURN(retVal);
        outAlloc.ptr = clEnqueueMapBuffer(outAlloc.queue, outAlloc.memObject, CL_BLOCKING,
                                          CL_MAP_READ | CL_MAP_WRITE, 0, size, 0u,
                                          nullptr, nullptr, &retVal);
        CL_SUCCESS_OR_RETURN(retVal);
        break;
    default:
        FATAL_ERROR("Unknown reuse mode");
    }

    return CL_SUCCESS;
}

cl_int HostptrReuseHelper::deallocateBufferHostptr(Alloc alloc) {
    switch (alloc.reuseMode) {
    case HostptrReuseMode::None:
        delete[](static_cast<uint8_t *>(alloc.ptr));
        break;
    case HostptrReuseMode::Usm:
        CL_SUCCESS_OR_RETURN(alloc.clMemFreeINTEL(alloc.context, alloc.ptr));
        break;
    case HostptrReuseMode::Map:
        CL_SUCCESS_OR_RETURN(clEnqueueUnmapMemObject(alloc.queue, alloc.memObject, alloc.ptr, 0, nullptr, nullptr));
        CL_SUCCESS_OR_RETURN(clReleaseMemObject(alloc.memObject));
        break;
    default:
        FATAL_ERROR("Unknown reuse mode");
    }

    return CL_SUCCESS;
}
