#pragma once

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <exception>
#include <gtest/gtest.h>
#include <iostream>

#define ASSERT_CL_SUCCESS(retVal)      \
    if (retVal != CL_SUCCESS) {        \
        EXPECT_EQ(CL_SUCCESS, retVal); \
        return true;                   \
    }

#define EXPECT_CL_SUCCESS(retVal)      \
    if (retVal != CL_SUCCESS) {        \
        EXPECT_EQ(CL_SUCCESS, retVal); \
    }

using cl_mem_properties_intel = cl_bitfield;

typedef CL_API_ENTRY void *(CL_API_CALL *pfn_clHostMemAllocINTEL)(
    cl_context context,
    const cl_mem_properties_intel *properties,
    size_t size,
    cl_uint alignment,
    cl_int *errcode_ret);

typedef CL_API_ENTRY void *(CL_API_CALL *pfn_clMemFreeINTEL)(
    cl_context context,
    const void *ptr);

struct Opencl {
    Opencl() {
        cl_uint numPlatforms;
        EXPECT_CL_SUCCESS(clGetPlatformIDs(0, nullptr, &numPlatforms));
        if (numPlatforms == 0) {
            throw std::exception();
        }

        EXPECT_CL_SUCCESS(clGetPlatformIDs(1, &platform, nullptr));

        cl_uint numDevices;
        EXPECT_CL_SUCCESS(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &numDevices));
        EXPECT_CL_SUCCESS(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr));

        cl_int retVal{};
        context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &retVal);
        EXPECT_CL_SUCCESS(retVal);

        commandQueue = clCreateCommandQueueWithProperties(context, device, nullptr, &retVal);
        EXPECT_CL_SUCCESS(retVal);
    }

    ~Opencl() {
        EXPECT_CL_SUCCESS(clReleaseCommandQueue(commandQueue));
        EXPECT_CL_SUCCESS(clReleaseContext(context));
    }

    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue commandQueue;
};
