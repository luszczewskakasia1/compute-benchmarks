#pragma once

#include "framework/configuration.h"
#include "framework/l0/context_properties.h"
#include "framework/l0/error.h"
#include "framework/l0/queue_properties.h"
#include "framework/utility/error.h"

#include <level_zero/ze_api.h>

namespace L0 {
struct LevelZero {
    ze_driver_handle_t driver{};
    ze_device_handle_t device{}; // default device, it is not present in multitile scenarios
    ze_context_handle_t context{};
    ze_command_queue_handle_t commandQueue{};
    ze_command_queue_desc_t commandQueueDesc{};
    ze_device_handle_t commandQueueDevice{};
    size_t commandQueueMaxFillSize{};

    LevelZero() : LevelZero(QueueProperties::create()) {}
    LevelZero(const QueueProperties &queueProperties) : LevelZero(queueProperties, ContextProperties::create()) {}
    LevelZero(const QueueProperties &queueProperties, const ContextProperties &contextProperties) {
        EXPECT_ZE_RESULT_SUCCESS(zeInit(ZE_INIT_FLAG_GPU_ONLY));

        // Get driver
        uint32_t driverCount = 0;
        EXPECT_ZE_RESULT_SUCCESS(zeDriverGet(&driverCount, nullptr));
        const auto driverIndex = ::configuration.l0DriverIndex;
        if (driverIndex >= driverCount) {
            ERROR("Invalid LevelZero driver selected");
        }
        auto drivers = std::make_unique<ze_driver_handle_t[]>(driverCount);
        EXPECT_ZE_RESULT_SUCCESS(zeDriverGet(&driverCount, drivers.get()));
        this->driver = drivers[driverIndex];

        // Create root device
        uint32_t deviceCount = 0;
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGet(driver, &deviceCount, nullptr));
        const auto deviceIndex = ::configuration.l0DeviceIndex;
        if (deviceIndex >= deviceCount) {
            ERROR("Invalid LevelZero device selected");
        }
        auto devices = std::make_unique<ze_device_handle_t[]>(deviceCount);
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGet(driver, &deviceCount, devices.get()));
        this->rootDevice = devices[deviceIndex];

        // Create subDevices if needed
        if (DeviceSelectionHelper::hasAnySubDevice(contextProperties.deviceSelection)) {
            createSubDevices(contextProperties.requireCreationSuccess);
            if (this->subDevices.size() == 0) {
                return;
            }
        }

        // Set the default device
        this->device = getDefaultDevice(contextProperties.deviceSelection);

        // Create context
        const ze_context_desc_t contextDesc{ZE_STRUCTURE_TYPE_CONTEXT_DESC};
        EXPECT_ZE_RESULT_SUCCESS(zeContextCreate(driver, &contextDesc, &context));

        // Create queue
        const auto queueCreationResults = createQueue(queueProperties);
        this->commandQueue = std::get<ze_command_queue_handle_t>(queueCreationResults);
        this->commandQueueDesc = std::get<ze_command_queue_desc_t>(queueCreationResults);
        this->commandQueueDevice = std::get<ze_device_handle_t>(queueCreationResults);
        this->commandQueueMaxFillSize = std::get<size_t>(queueCreationResults);
    }

    ~LevelZero() {
        if (commandQueue != nullptr) {
            EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueDestroy(commandQueue));
        }
        EXPECT_ZE_RESULT_SUCCESS(zeContextDestroy(context));
    }

    ze_device_handle_t getDevice(DeviceSelection deviceSelection) const {
        ERROR_IF(DeviceSelectionHelper::hasHost(deviceSelection), "Cannot get ze_device_handle_t for host");
        ERROR_UNLESS(DeviceSelectionHelper::hasSingleDevice(deviceSelection), "Cannot get multiple devices");
        if (deviceSelection == DeviceSelection::Root) {
            return this->rootDevice;
        }

        const auto subDeviceIndex = DeviceSelectionHelper::getSubDeviceIndex(deviceSelection);
        ERROR_UNLESS((subDeviceIndex < this->subDevices.size()), "Invalid subDevice index");
        return this->subDevices[subDeviceIndex];
    }

    ze_device_properties_t getDeviceProperties() const { return getDeviceProperties(this->device); }
    ze_device_properties_t getDeviceProperties(DeviceSelection deviceSelection) const { return getDeviceProperties(getDevice(deviceSelection)); }
    ze_device_properties_t getDeviceProperties(ze_device_handle_t device) const {
        ze_device_properties_t deviceProperties{ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES};
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetProperties(device, &deviceProperties));
        return deviceProperties;
    }
    uint64_t getTimerResoultion(DeviceSelection deviceSelection) const { return getDeviceProperties(deviceSelection).timerResolution; }
    uint64_t getTimerResoultion(ze_device_handle_t device) const { return getDeviceProperties(device).timerResolution; }

  private:
    void createSubDevices(bool requireSuccess) {
        uint32_t numSubDevices{};
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetSubDevices(this->rootDevice, &numSubDevices, nullptr));
        if (numSubDevices == 0) {
            ERROR_IF(requireSuccess, "SubDevice was selected, but device has 0 subDevices");
            return;
        }

        subDevices.resize(numSubDevices);
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetSubDevices(this->rootDevice, &numSubDevices, subDevices.data()));
    }

    ze_device_handle_t getDefaultDevice(DeviceSelection deviceSelection) {
        if (DeviceSelectionHelper::hasSingleDevice(deviceSelection)) {
            return getDevice(deviceSelection);
        }
        return nullptr;
    }

    struct QueueDesc {
        bool isCopyOnly = {};
        size_t maxFillSize;
        ze_command_queue_desc_t desc = {};
    };
    static std::vector<QueueDesc> queryQueueFamilies(ze_device_handle_t device) {
        // Get queue ordinals
        uint32_t numQueueGroups = 0;
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetCommandQueueGroupProperties(device, &numQueueGroups, nullptr));
        ERROR_IF(numQueueGroups == 0, "No queue groups found!");
        std::vector<ze_command_queue_group_properties_t> queueProperties(numQueueGroups);
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetCommandQueueGroupProperties(device, &numQueueGroups, queueProperties.data()));

        // Iterate over queue groups
        std::vector<QueueDesc> result{};
        for (uint32_t i = 0; i < numQueueGroups; i++) {
            const bool isCompute = queueProperties[i].flags & ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE;
            const bool isCopy = queueProperties[i].flags & ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COPY;
            const bool isCopyOnly = !isCompute && isCopy;
            const size_t maxFillSize = queueProperties[i].maxMemoryFillPatternSize;

            // Add compute and copy only queue
            if (isCopyOnly || isCompute) {
                ze_command_queue_desc_t desc = {};
                desc.stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC;
                desc.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS;
                desc.ordinal = i;
                result.push_back({isCopyOnly, maxFillSize, desc});
            }
        }
        return result;
    }

    std::tuple<ze_command_queue_handle_t, ze_command_queue_desc_t, ze_device_handle_t, size_t> createQueue(const QueueProperties &queueProperties) {
        if (!queueProperties.createQueue) {
            return {};
        }

        // Get device
        const ze_device_handle_t deviceForQueue = getDevice(queueProperties.deviceSelection);

        // Get desc
        const auto queueFamilies = queryQueueFamilies(deviceForQueue);
        auto descEntry = std::find_if(queueFamilies.begin(), queueFamilies.end(), [queueProperties](const QueueDesc &d) { return d.isCopyOnly == queueProperties.copyQueue; });
        if (descEntry == queueFamilies.end()) {
            ERROR_IF(queueProperties.requireCreationSuccess, "Device does not support such queue");
            return {};
        }
        const ze_command_queue_desc_t commandQueueDesc = descEntry->desc;

        // Create
        ze_command_queue_handle_t commandQueue = {};
        EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueCreate(this->context, deviceForQueue, &commandQueueDesc, &commandQueue));
        return std::make_tuple(commandQueue, commandQueueDesc, deviceForQueue, descEntry->maxFillSize);
    }

    ze_device_handle_t rootDevice{};
    std::vector<ze_device_handle_t> subDevices{};
};
} // namespace L0

using namespace L0;
