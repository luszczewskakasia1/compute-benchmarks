#pragma once

#include "framework/ocl/context_properties.h"
#include "framework/ocl/function_signatures_ocl.h"
#include "framework/ocl/queue_properties.h"
#include "framework/ocl/utility/error.h"
#include "framework/ocl/utility/extensions_helper.h"
#include "framework/test_case/test_case.h"

namespace OCL {
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
        const auto platformIndex = Configuration::get().oclPlatformIndex;
        if (platformIndex >= numPlatforms) {
            FATAL_ERROR("Invalid OCL platform index. platformIndex=", platformIndex, " numPlatforms=", numPlatforms);
        }
        auto platforms = std::make_unique<cl_platform_id[]>(numPlatforms);
        EXPECT_CL_SUCCESS(clGetPlatformIDs(numPlatforms, platforms.get(), nullptr));
        this->platform = platforms[platformIndex];

        // Create root device
        cl_uint numDevices;
        EXPECT_CL_SUCCESS(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &numDevices));
        const auto deviceIndex = Configuration::get().oclDeviceIndex;
        if (deviceIndex >= numDevices) {
            FATAL_ERROR("Invalid OCL device index. deviceIndex=", deviceIndex, " numDevices=", numDevices);
        }
        auto devices = std::make_unique<cl_device_id[]>(numDevices);
        EXPECT_CL_SUCCESS(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices.get(), nullptr));
        this->rootDevice = devices[deviceIndex];

        // Create sub devices if needed
        if (DeviceSelectionHelper::hasAnySubDevice(contextProperties.deviceSelection)) {
            this->createSubDevices(contextProperties.requireCreationSuccess);
            const auto requiredSubDevicesCount = DeviceSelectionHelper::getMaxSubDeviceIndex(contextProperties.deviceSelection) + 1;
            if (this->subDevices.size() < requiredSubDevicesCount) {
                return;
            }
        }

        // Set the default device
        if (DeviceSelectionHelper::hasSingleDevice(contextProperties.deviceSelection)) {
            this->device = getDevice(contextProperties.deviceSelection);
        }

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

    cl_command_queue createQueue(QueueProperties queueProperties) {
        if (!queueProperties.createQueue) {
            return nullptr;
        }
        const cl_device_id deviceForQueue = getDevice(queueProperties.deviceSelection);
        const size_t maxPropertiesCount = 7u;
        cl_int retVal{};
        cl_command_queue queue{};
        cl_queue_properties properties[maxPropertiesCount] = {};
        const bool needsQueueSelection = queueProperties.forceBlitter;

        // Force legacy path, if the new path is not supported
        if (needsQueueSelection && !getExtensions().isCommandQueueFamiliesSupported()) {
            queueProperties.setUseLegacyQueueFamilySelection(true);
        }

        // Create queue
        if (queueProperties.fillQueueProperties(deviceForQueue, properties, maxPropertiesCount)) {
            queue = clCreateCommandQueueWithProperties(this->context, deviceForQueue, properties, &retVal);
        }

        // If family selection with cl_intel_queue_families failed, try the legacy path
        if (queue == nullptr && needsQueueSelection && !queueProperties.useLegacyQueueFamilySelection) {
            queueProperties.setUseLegacyQueueFamilySelection(true);
            if (queueProperties.fillQueueProperties(deviceForQueue, properties, maxPropertiesCount)) {
                queue = clCreateCommandQueueWithProperties(this->context, deviceForQueue, properties, &retVal);
            }
        }

        if (queueProperties.requireCreationSuccess) {
            CL_SUCCESS_OR_ERROR(retVal, "Command queue creation failed");
        }

        if (queue) {
            this->commandQueues.push_back(queue);
        }
        return queue;
    }

    cl_device_id getDevice(DeviceSelection deviceSelection) {
        FATAL_ERROR_IF(DeviceSelectionHelper::hasHost(deviceSelection), "Cannot get cl_device_id for host");
        FATAL_ERROR_UNLESS(DeviceSelectionHelper::hasSingleDevice(deviceSelection), "Cannot get multiple devices");
        if (deviceSelection == DeviceSelection::Root) {
            return this->rootDevice;
        }

        const auto subDeviceIndex = DeviceSelectionHelper::getSubDeviceIndex(deviceSelection);
        FATAL_ERROR_UNLESS((subDeviceIndex < this->subDevices.size()), "Invalid subDevice index");
        return this->subDevices[subDeviceIndex];
    }

    const ExtensionsHelper &getExtensions() {
        if (extensionsHelper == nullptr) {
            extensionsHelper = std::make_unique<ExtensionsHelper>(this->rootDevice);
        }
        return *extensionsHelper;
    }

  private:
    cl_context createContext(const ContextProperties &contextProperties) {
        std::vector<cl_device_id> devicesForContext = getDevices(contextProperties.deviceSelection, false);
        if (devicesForContext.size() == 0) {
            FATAL_ERROR_IF(contextProperties.requireCreationSuccess, "Failed getting devices for context");
            return nullptr;
        }

        cl_int retVal{};
        cl_context context = clCreateContext(nullptr, static_cast<cl_uint>(devicesForContext.size()), devicesForContext.data(), nullptr, nullptr, &retVal);
        if (contextProperties.requireCreationSuccess) {
            CL_SUCCESS_OR_ERROR(retVal, "Context creation failed");
        }
        return context;
    }

    std::vector<cl_device_id> getDevices(DeviceSelection deviceSelection, bool requireSuccess) {
        FATAL_ERROR_IF(DeviceSelectionHelper::hasHost(deviceSelection), "Cannot get cl_device_id for host");
        std::vector<cl_device_id> result = {};

        // Add root device
        if ((deviceSelection & DeviceSelection::Root) == DeviceSelection::Root) {
            FATAL_ERROR_IF(this->rootDevice == nullptr, "Root device has not been created yet");
            result.push_back(this->rootDevice);
        }

        // Add subDevices
        for (DeviceSelection subDevice : DeviceSelectionHelper::subDevices) {
            if (DeviceSelectionHelper::hasDevice(deviceSelection, subDevice)) {
                const auto subDeviceIndex = DeviceSelectionHelper::getSubDeviceIndex(subDevice);
                FATAL_ERROR_IF(subDeviceIndex >= subDevices.size() && requireSuccess, "Invalid subDevice selected")
                result.push_back(subDevices[subDeviceIndex]);
            }
        }

        // Validate number of devices
        const auto expectedCount = DeviceSelectionHelper::getDevicesCount(deviceSelection);
        const auto actualCount = result.size();
        if (expectedCount != actualCount) {
            FATAL_ERROR_IF(expectedCount != actualCount && requireSuccess, "Invalid number of devices accumulated");
            return {};
        }

        return result;
    }

    bool createSubDevices(bool requireSuccess) {
        if (subDevices.size() != 0) {
            return true;
        }

        cl_device_affinity_domain domain{};
        EXPECT_CL_SUCCESS(clGetDeviceInfo(this->rootDevice, CL_DEVICE_PARTITION_AFFINITY_DOMAIN, sizeof(domain), &domain, NULL));
        if ((domain & CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE) == 0) {
            FATAL_ERROR_IF(requireSuccess, "SubDevice was selected, but device is not partitionable");
            return false;
        }
        if ((domain & CL_DEVICE_AFFINITY_DOMAIN_NUMA) == 0) {
            FATAL_ERROR_IF(requireSuccess, "SubDevice was selected, but device is not CL_DEVICE_AFFINITY_DOMAIN_NUMA");
            return false;
        }

        const cl_device_partition_property properties[] = {CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN, CL_DEVICE_AFFINITY_DOMAIN_NUMA, 0};
        cl_uint numSubDevices{};
        EXPECT_CL_SUCCESS(clCreateSubDevices(this->rootDevice, properties, 0, nullptr, &numSubDevices));
        this->subDevices.resize(numSubDevices);
        EXPECT_CL_SUCCESS(clCreateSubDevices(this->rootDevice, properties, numSubDevices, this->subDevices.data(), nullptr));
        return true;
    }

    cl_device_id rootDevice;
    std::vector<cl_device_id> subDevices{};
    std::vector<cl_command_queue> commandQueues{};
    std::unique_ptr<ExtensionsHelper> extensionsHelper{};
};

} // namespace OCL
using namespace OCL;
