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

#include "framework/ocl/cl.h"

typedef CL_API_ENTRY void *(CL_API_CALL *pfn_clHostMemAllocINTEL)(
    cl_context context,
    const cl_mem_properties_intel *properties,
    size_t size,
    cl_uint alignment,
    cl_int *errcode_ret);

typedef CL_API_ENTRY void *(CL_API_CALL *pfn_clDeviceMemAllocINTEL)(
    cl_context context,
    cl_device_id device,
    const cl_mem_properties_intel *properties,
    size_t size,
    cl_uint alignment,
    cl_int *errcode_ret);

typedef CL_API_ENTRY void *(CL_API_CALL *pfn_clSharedMemAllocINTEL)(
    cl_context context,
    cl_device_id device,
    const cl_mem_properties_intel *properties,
    size_t size,
    cl_uint alignment,
    cl_int *errcodeRet);

typedef CL_API_ENTRY cl_int(CL_API_CALL *pfn_clMemFreeINTEL)(
    cl_context context,
    const void *ptr);

typedef CL_API_ENTRY cl_int(CL_API_CALL *pfn_clEnqueueMemcpyINTEL)(
    cl_command_queue commandQueue,
    cl_bool blocking,
    void *dstPtr,
    const void *srcPtr,
    size_t size,
    cl_uint numEventsInWaitList,
    const cl_event *eventWaitList,
    cl_event *event);

typedef CL_API_ENTRY cl_int(CL_API_CALL *pfn_clEnqueueMemFillINTEL)(
    cl_command_queue commandQueue,
    void *dstPtr,
    const void *pattern,
    size_t patternSize,
    size_t size,
    cl_uint numEventsInWaitList,
    const cl_event *eventWaitList,
    cl_event *event);

typedef CL_API_ENTRY cl_int(CL_API_CALL *pfn_clEnqueueMemsetINTEL)(
    cl_command_queue commandQueue,
    void *dstPtr,
    cl_int value,
    size_t size,
    cl_uint numEventsInWaitList,
    const cl_event *eventWaitList,
    cl_event *event);

typedef CL_API_ENTRY cl_mem(CL_API_CALL *pfn_clCreateBufferWithPropertiesINTEL)(
    cl_context context,
    const cl_mem_properties_intel *properties,
    cl_mem_flags flags,
    size_t size,
    void *hostPtr,
    cl_int *errcodeRet);
