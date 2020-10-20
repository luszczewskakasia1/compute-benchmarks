#pragma once

#include "framework/ocl/device_properties.h"
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
    Opencl(const QueueProperties &queueProperties) : Opencl(queueProperties, DeviceProperties::create()) {}
    Opencl(const QueueProperties &queueProperties, const DeviceProperties &deviceProperties) {
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

        // Create a default device (either a root device or one of the subDevices)
        this->device = getDevice(deviceProperties.subDeviceSelection, deviceProperties.requireSubDeviceCreationSuccess);
        if (this->device == nullptr) {
            return;
        }

        // Create context on the default device
        this->context = createContext(deviceProperties.subDeviceSelection);
        if (this->context == nullptr) {
            return;
        }

        // Create command queue on the default
        createQueue(queueProperties);
    }

    ~Opencl() {
        if (commandQueue != nullptr) {
            EXPECT_CL_SUCCESS(clReleaseCommandQueue(commandQueue));
        }
        for (auto &context : contexts) {
            EXPECT_CL_SUCCESS(clReleaseContext(context));
        }
        for (auto &subDevice : subDevices) {
            EXPECT_CL_SUCCESS(clReleaseDevice(subDevice));
        }
    }

    void createQueue(const QueueProperties &queueProperties) {
        if (queueProperties.createQueue) {
            cl_int retVal{};
            this->commandQueue = clCreateCommandQueueWithProperties(context, device, queueProperties, &retVal);
            if (queueProperties.requireCreationSuccess) {
                EXPECT_CL_SUCCESS(retVal);
            }
        }
    }

    void createSubDevices(bool requireSuccess) {
        cl_device_affinity_domain domain{};
        EXPECT_CL_SUCCESS(clGetDeviceInfo(this->rootDevice, CL_DEVICE_PARTITION_AFFINITY_DOMAIN, sizeof(domain), &domain, NULL));
        if ((domain & CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE) == 0) {
            ERROR_IF(requireSuccess, "SubDevice was selected, but device is not partitionable");
            return;
        }
        if ((domain & CL_DEVICE_AFFINITY_DOMAIN_NUMA) == 0) {
            ERROR_IF(requireSuccess, "SubDevice was selected, but device is not CL_DEVICE_AFFINITY_DOMAIN_NUMA");
            return;
        }

        const cl_device_partition_property properties[] = {CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN, CL_DEVICE_AFFINITY_DOMAIN_NUMA, 0};
        cl_uint numSubDevices{};
        EXPECT_CL_SUCCESS(clCreateSubDevices(this->rootDevice, properties, 0, nullptr, &numSubDevices));
        this->subDevices.resize(numSubDevices);
        EXPECT_CL_SUCCESS(clCreateSubDevices(this->rootDevice, properties, numSubDevices, this->subDevices.data(), nullptr));
    }

    cl_context createContext(DeviceSelection subDeviceSelection) {
        cl_device_id deviceForContext = getDevice(subDeviceSelection, false);
        if (deviceForContext == nullptr) {
            return nullptr;
        }

        cl_int retVal{};
        cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &retVal);
        EXPECT_CL_SUCCESS(retVal);
        contexts.push_back(context);
        return context;
    }

    cl_platform_id platform{};
    cl_device_id device{};
    cl_context context{};
    cl_command_queue commandQueue{};

  private:
    cl_device_id getDevice(DeviceSelection subDeviceSelection, bool requireSuccess) {
        if (subDeviceSelection == DeviceSelection::Root) {
            ERROR_IF(!rootDevice && requireSuccess, "Root device has not been created yet");
            return rootDevice;
        }

        if (subDevices.size() == 0) {
            createSubDevices(requireSuccess);
        }

        const auto subDeviceIndex = getSubDeviceIndexFromDeviceSelection(subDeviceSelection);
        if (subDeviceIndex >= subDevices.size()) {
            ERROR_IF(requireSuccess, "Invalid subDevice selected");
            return nullptr;
        }

        return subDevices[subDeviceIndex];
    }

    cl_device_id rootDevice;
    std::vector<cl_device_id> subDevices{};
    std::vector<cl_context> contexts{};
};
