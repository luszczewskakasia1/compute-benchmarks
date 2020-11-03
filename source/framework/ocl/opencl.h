#pragma once

#include "framework/ocl/context_properties.h"
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
    cl_platform_id platform{};
    cl_device_id device{};
    cl_context context{};
    cl_command_queue commandQueue{};

    Opencl() : Opencl(QueueProperties::create()) {}
    Opencl(const QueueProperties &queueProperties) : Opencl(queueProperties, ContextProperties::create()) {}
    Opencl(const QueueProperties &queueProperties, const ContextProperties &contextProperties) {
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

        // Create sub devices if needed
        if (DeviceSelectionHelper::hasAnySubDevice(queueProperties.deviceSelection) ||
            DeviceSelectionHelper::hasAnySubDevice(contextProperties.deviceSelection)) {
            createSubDevices();
        }

        // Set the default device
        this->device = getDefaultDevice(contextProperties.deviceSelection);

        // Create context on the default device
        this->context = createContext(contextProperties);
        if (this->context == nullptr) {
            return;
        }

        // Create command queue on the default device
        this->commandQueue = createQueue(queueProperties);
    }

    ~Opencl() {
        for (auto &commandQueue : commandQueues) {
            EXPECT_CL_SUCCESS(clReleaseCommandQueue(commandQueue));
        }
        if (context) {
            EXPECT_CL_SUCCESS(clReleaseContext(context));
        }
        for (auto &subDevice : subDevices) {
            EXPECT_CL_SUCCESS(clReleaseDevice(subDevice));
        }
    }

    cl_command_queue createQueue(const QueueProperties &queueProperties) {
        if (!queueProperties.createQueue) {
            return nullptr;
        }

        cl_int retVal{};
        cl_command_queue queue = clCreateCommandQueueWithProperties(this->context, this->device, queueProperties, &retVal);
        if (queueProperties.requireCreationSuccess) {
            EXPECT_CL_SUCCESS(retVal);
        }

        if (queue) {
            this->commandQueues.push_back(queue);
        }
        return queue;
    }

    cl_device_id getDevice(DeviceSelection deviceSelection) {
        ERROR_UNLESS(DeviceSelectionHelper::hasSingleDevice(deviceSelection), "Cannot get multiple devices");
        if (deviceSelection == DeviceSelection::Root) {
            return this->rootDevice;
        }

        const auto subDeviceIndex = DeviceSelectionHelper::getSubDeviceIndex(deviceSelection);
        ERROR_UNLESS((subDeviceIndex < this->subDevices.size()), "Invalid subDevice index");
        return this->subDevices[subDeviceIndex];
    }

  private:
    cl_context createContext(const ContextProperties &contextProperties) {
        std::vector<cl_device_id> devicesForContext = getDevices(contextProperties.deviceSelection, false);
        if (devicesForContext.size() == 0) {
            ERROR_IF(contextProperties.requireCreationSuccess, "Failed getting devices for context");
            return nullptr;
        }

        cl_int retVal{};
        cl_context context = clCreateContext(nullptr, static_cast<cl_uint>(devicesForContext.size()), devicesForContext.data(), nullptr, nullptr, &retVal);
        if (contextProperties.requireCreationSuccess) {
            EXPECT_CL_SUCCESS(retVal);
        }
        return context;
    }

    std::vector<cl_device_id> getDevices(DeviceSelection deviceSelection, bool requireSuccess) {
        std::vector<cl_device_id> result = {};

        // Add root device
        if ((deviceSelection & DeviceSelection::Root) == DeviceSelection::Root) {
            ERROR_IF(this->rootDevice == nullptr, "Root device has not been created yet");
            result.push_back(this->rootDevice);
        }

        // Add subDevices
        for (DeviceSelection subDevice : DeviceSelectionHelper::subDevices) {
            if (DeviceSelectionHelper::hasDevice(deviceSelection, subDevice)) {
                const auto subDeviceIndex = DeviceSelectionHelper::getSubDeviceIndex(subDevice);
                ERROR_IF(subDeviceIndex >= subDevices.size() && requireSuccess, "Invalid subDevice selected")
                result.push_back(subDevices[subDeviceIndex]);
            }
        }

        // Validate number of devices
        const auto expectedCount = DeviceSelectionHelper::getDevicesCount(deviceSelection);
        const auto actualCount = result.size();
        if (expectedCount != actualCount) {
            ERROR_IF(expectedCount != actualCount && requireSuccess, "Invalid number of devices accumulated");
            return {};
        }

        return result;
    }

    void createSubDevices() {
        cl_device_affinity_domain domain{};
        EXPECT_CL_SUCCESS(clGetDeviceInfo(this->rootDevice, CL_DEVICE_PARTITION_AFFINITY_DOMAIN, sizeof(domain), &domain, NULL));
        if ((domain & CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE) == 0) {
            ERROR("SubDevice was selected, but device is not partitionable");
            return;
        }
        if ((domain & CL_DEVICE_AFFINITY_DOMAIN_NUMA) == 0) {
            ERROR("SubDevice was selected, but device is not CL_DEVICE_AFFINITY_DOMAIN_NUMA");
            return;
        }

        const cl_device_partition_property properties[] = {CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN, CL_DEVICE_AFFINITY_DOMAIN_NUMA, 0};
        cl_uint numSubDevices{};
        EXPECT_CL_SUCCESS(clCreateSubDevices(this->rootDevice, properties, 0, nullptr, &numSubDevices));
        this->subDevices.resize(numSubDevices);
        EXPECT_CL_SUCCESS(clCreateSubDevices(this->rootDevice, properties, numSubDevices, this->subDevices.data(), nullptr));
    }

    cl_device_id getDefaultDevice(DeviceSelection deviceSelection) {
        if (DeviceSelectionHelper::hasSingleDevice(deviceSelection)) {
            return getDevice(deviceSelection);
        }
        return nullptr;
    }

    cl_device_id rootDevice;
    std::vector<cl_device_id> subDevices{};
    std::vector<cl_command_queue> commandQueues{};
};
