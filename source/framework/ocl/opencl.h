#pragma once

#include "framework/test_case/test_case.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <CL/embargo/cl_ext_private_internal.h>
#include <exception>
#include <gtest/gtest.h>
#include <iostream>

#define CL_MEM_USES_COMPRESSION_INTEL (0x10051)

#define ASSERT_CL_SUCCESS(retVal)      \
    if (retVal != CL_SUCCESS) {        \
        EXPECT_EQ(CL_SUCCESS, retVal); \
        return TestResult::Error;      \
    }

#define EXPECT_CL_SUCCESS(retVal)      \
    if (retVal != CL_SUCCESS) {        \
        EXPECT_EQ(CL_SUCCESS, retVal); \
    }

#define CL_SUCCESS_OR_TERMINATE(retVal)                                                      \
    if (retVal != CL_SUCCESS) {                                                              \
        ERROR(std::string("Fatal OpenCL error occurred, retVal=") + std::to_string(retVal)); \
    }

using cl_mem_properties_intel = cl_bitfield;

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

struct Opencl {
    Opencl() : Opencl(true) {}
    Opencl(bool createQueue) {
        // Get Platform
        cl_uint numPlatforms;
        EXPECT_CL_SUCCESS(clGetPlatformIDs(0, nullptr, &numPlatforms));
        const auto platformIndex = ::configuration.oclPlatformIndex;
        if (platformIndex >= numPlatforms) {
            ERROR("Invalid platform selected");
        }
        auto platforms = std::make_unique<cl_platform_id[]>(numPlatforms);
        EXPECT_CL_SUCCESS(clGetPlatformIDs(numPlatforms, platforms.get(), nullptr));
        this->platform = platforms[platformIndex];

        // Get Device
        cl_uint numDevices;
        EXPECT_CL_SUCCESS(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &numDevices));
        const auto deviceIndex = ::configuration.oclDeviceIndex;
        if (deviceIndex >= numDevices) {
            ERROR("Invalid device selected");
        }
        auto devices = std::make_unique<cl_device_id[]>(numDevices);
        EXPECT_CL_SUCCESS(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices.get(), nullptr));
        this->device = devices[deviceIndex];

        // Create context
        cl_int retVal{};
        context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &retVal);
        EXPECT_CL_SUCCESS(retVal);

        // Get command queue properties
        cl_command_queue_properties supportedProperties{};
        EXPECT_CL_SUCCESS(clGetDeviceInfo(device, CL_DEVICE_QUEUE_ON_HOST_PROPERTIES, sizeof(supportedProperties), &supportedProperties, nullptr));
        if (::configuration.oclUseOOQ && (supportedProperties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)) {
            this->queueProperties[1] |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
            this->profilingQueueProperties[1] |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
        }

        // Create command queue
        if (createQueue) {
            commandQueue = clCreateCommandQueueWithProperties(context, device, this->queueProperties, &retVal);
            EXPECT_CL_SUCCESS(retVal);
        }
    }

    ~Opencl() {
        if (commandQueue != nullptr) {
            EXPECT_CL_SUCCESS(clReleaseCommandQueue(commandQueue));
        }
        EXPECT_CL_SUCCESS(clReleaseContext(context));
    }

    cl_platform_id platform{};
    cl_device_id device{};
    cl_context context{};
    cl_command_queue_properties queueProperties[3] = {CL_QUEUE_PROPERTIES, 0, 0};
    cl_command_queue_properties profilingQueueProperties[3] = {CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0};
    cl_command_queue commandQueue{};

    static cl_mem_flags getCompressionFlags(bool compression, bool noIntelExtensions) {
        if (noIntelExtensions) {
            return 0;
        }

        if (compression) {
            return CL_MEM_COMPRESSED_HINT_INTEL;
        } else {
            return CL_MEM_UNCOMPRESSED_HINT_INTEL;
        }
    }

    static TestResult verifyCompression(cl_mem mem, bool expectedCompression, bool noIntelExtensions) {
        if (noIntelExtensions) {
            return TestResult::Success; // We have nothing to check, this is a vendor agnostic benchmark run
        }

        cl_bool isCompressedRaw{};
        const cl_int retVal = clGetMemObjectInfo(mem, CL_MEM_USES_COMPRESSION_INTEL, sizeof(isCompressedRaw), &isCompressedRaw, nullptr);
        if (retVal != CL_SUCCESS) {
            return TestResult::Error; // We are not on an Intel GPU. Parameter --no-intel-extensions should be passed
        }

        const bool isCompressed = isCompressedRaw == CL_TRUE;
        if (isCompressed != expectedCompression) {
            return TestResult::DeviceNotCapable; // We are on an Intel GPU, which does not support compression, e.g. Skylake
        }
        return TestResult::Success;
    }
};
