#pragma once

#include "framework/ocl/function_signatures_ocl.h"
#include "framework/ocl/queue_properties.h"
#include "framework/test_case/test_case.h"

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

struct Opencl {
    Opencl() : Opencl(QueueProperties::create()) {}
    Opencl(const QueueProperties &queueProperties, bool requireQueueCreationSuccess = true) : Opencl(&queueProperties, requireQueueCreationSuccess) {}
    Opencl(const QueueProperties *queueProperties, bool requireQueueCreationSuccess) {
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

        // Create root device
        cl_uint numDevices;
        EXPECT_CL_SUCCESS(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &numDevices));
        const auto deviceIndex = ::configuration.oclDeviceIndex;
        if (deviceIndex >= numDevices) {
            ERROR("Invalid device selected");
        }
        auto devices = std::make_unique<cl_device_id[]>(numDevices);
        EXPECT_CL_SUCCESS(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices.get(), nullptr));
        this->rootDevice = devices[deviceIndex];
        this->device = this->rootDevice;

        // Create subDevices if needed
        const auto subDeviceSelection = ::configuration.subDeviceSelection;
        if (subDeviceSelection != DeviceSelection::Root) {
            createSubDevices();

            const auto subDeviceIndex = getSubDeviceIndexFromDeviceSelection(subDeviceSelection);
            if (subDeviceIndex >= subDevices.size()) {
                ERROR("Invalid subDevice selected");
            }

            this->device = this->subDevices[subDeviceIndex];
        }

        // Create context
        cl_int retVal{};
        context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &retVal);
        EXPECT_CL_SUCCESS(retVal);

        // Create command queue
        if (queueProperties != nullptr) {
            retVal = createQueue(*queueProperties);
            if (requireQueueCreationSuccess) {
                EXPECT_CL_SUCCESS(retVal);
            }
        }
    }

    ~Opencl() {
        if (commandQueue != nullptr) {
            EXPECT_CL_SUCCESS(clReleaseCommandQueue(commandQueue));
        }
        EXPECT_CL_SUCCESS(clReleaseContext(context));
        for (auto &subDevice : subDevices) {
            EXPECT_CL_SUCCESS(clReleaseDevice(subDevice));
        }
    }

    cl_int createQueue(const QueueProperties &queueProperties) {
        cl_int retVal{};
        this->commandQueue = clCreateCommandQueueWithProperties(context, device, queueProperties, &retVal);
        return retVal;
    }

    void createSubDevices() {
        cl_device_affinity_domain domain{};
        EXPECT_CL_SUCCESS(clGetDeviceInfo(device, CL_DEVICE_PARTITION_AFFINITY_DOMAIN, sizeof(domain), &domain, NULL));
        if ((domain & CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE) == 0) {
            ERROR("SubDevice was selected, but device is not partitionable");
        }
        if ((domain & CL_DEVICE_AFFINITY_DOMAIN_NUMA) == 0) {
            ERROR("SubDevice was selected, but device is not CL_DEVICE_AFFINITY_DOMAIN_NUMA");
        }

        const cl_device_partition_property properties[] = {CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN, CL_DEVICE_AFFINITY_DOMAIN_NUMA, 0};
        cl_uint numSubDevices{};
        EXPECT_CL_SUCCESS(clCreateSubDevices(this->rootDevice, properties, 0, nullptr, &numSubDevices));
        if (numSubDevices == 0) {
            ERROR("SubDevice was selected, but device has 0 subDevices");
        }

        this->subDevices.resize(numSubDevices);
        EXPECT_CL_SUCCESS(clCreateSubDevices(this->rootDevice, properties, numSubDevices, this->subDevices.data(), nullptr));
    }

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

    cl_platform_id platform{};
    cl_device_id device{};
    cl_context context{};
    cl_command_queue commandQueue{};

  private:
    cl_device_id rootDevice;
    std::vector<cl_device_id> subDevices{};
};
