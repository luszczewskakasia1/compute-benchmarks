#pragma once

#include "framework/test_case/test_case.h"

// clang-format off
#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include "framework/ocl/function_signatures_ocl.h"
// clang-format on

#include <exception>
#include <gtest/gtest.h>
#include <iostream>

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

struct QueueProperties {
    cl_command_queue_properties properties[5] = {CL_QUEUE_PROPERTIES, 0, 0, 0, 0};

    operator const cl_command_queue_properties *() const {
        return &properties[0];
    }

    static QueueProperties create() {
        return create(false, false, -1);
    }

    static QueueProperties createProfilingOrNot(bool profiling) {
        return create(profiling, false, -1);
    }

    static QueueProperties createBcsOrNot(bool bcs) {
        return create(false, bcs, -1);
    }

    static QueueProperties createOoqOrNot(bool ooqArg) {
        const int ooq = ooqArg ? 1 : 0;
        return create(false, false, ooq);
    }

    static QueueProperties create(bool profiling, bool bcs, int ooq) {
        QueueProperties result{};
        if (profiling) {
            result.properties[1] |= CL_QUEUE_PROFILING_ENABLE;
        }
        if (bcs) {
            result.properties[2] = CL_QUEUE_FAMILY_INTEL;
            result.properties[3] = CL_QUEUE_FAMILY_TYPE_BCS_INTEL;
        }
        if (ooq == 1 || (ooq == -1 && ::configuration.oclUseOOQ)) {
            result.properties[1] |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
        }

        return result;
    }
};

struct Opencl {
    Opencl() : Opencl(QueueProperties::create()) {}
    Opencl(const QueueProperties &queueProperties) : Opencl(&queueProperties) {}
    Opencl(const QueueProperties *queueProperties) {
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

        // Create queueProperties
        if (queueProperties != nullptr) {
            EXPECT_CL_SUCCESS(createQueue(*queueProperties));
        }
    }

    ~Opencl() {
        if (commandQueue != nullptr) {
            EXPECT_CL_SUCCESS(clReleaseCommandQueue(commandQueue));
        }
        EXPECT_CL_SUCCESS(clReleaseContext(context));
    }

    cl_int createQueue(const QueueProperties &queueProperties) {
        cl_int retVal{};
        this->commandQueue = clCreateCommandQueueWithProperties(context, device, queueProperties, &retVal);
        return retVal;
    }

    cl_platform_id platform{};
    cl_device_id device{};
    cl_context context{};
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
