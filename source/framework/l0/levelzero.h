#pragma once

#include "framework/configuration.h"
#include "framework/utility/error.h"

#include <gtest/gtest.h>
#include <iostream>
#include <level_zero/ze_api.h>

#define ASSERT_ZE_RESULT_SUCCESS(retVal)      \
    if (retVal != ZE_RESULT_SUCCESS) {        \
        EXPECT_EQ(ZE_RESULT_SUCCESS, retVal); \
        return TestResult::Error;             \
    }

#define EXPECT_ZE_RESULT_SUCCESS(retVal)      \
    if (retVal != ZE_RESULT_SUCCESS) {        \
        EXPECT_EQ(ZE_RESULT_SUCCESS, retVal); \
    }

#define ZE_RESULT_SUCCESS_OR_TERMINATE(retVal)                                                  \
    if (retVal != ZE_RESULT_SUCCESS) {                                                          \
        ERROR(std::string("Fatal LevelZero error occurred, retVal=") + std::to_string(retVal)); \
    }

#define ZE_RESULT_SUCCESS_OR_RETURN_DEFAULT(retVal) \
    if (retVal != ZE_RESULT_SUCCESS) {              \
        EXPECT_EQ(ZE_RESULT_SUCCESS, retVal);       \
        return {};                                  \
    }

struct LevelZero {
    LevelZero() : LevelZero(true) {}
    LevelZero(bool createCommandQueue) {
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

        // Get device
        uint32_t deviceCount = 0;
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGet(driver, &deviceCount, nullptr));
        const auto deviceIndex = ::configuration.l0DeviceIndex;
        if (deviceIndex >= deviceCount) {
            ERROR("Invalid LevelZero device selected");
        }
        auto devices = std::make_unique<ze_device_handle_t[]>(deviceCount);
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGet(driver, &deviceCount, devices.get()));
        this->device = devices[deviceIndex];

        // Get device info
        this->deviceProperties.stype = {ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES};
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetProperties(this->device, &this->deviceProperties));

        // Create context
        const ze_context_desc_t contextDesc{ZE_STRUCTURE_TYPE_CONTEXT_DESC};
        EXPECT_ZE_RESULT_SUCCESS(zeContextCreate(driver, &contextDesc, &context));

        // Get queue ordinals
        uint32_t numQueueGroups = 0;
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetCommandQueueGroupProperties(device, &numQueueGroups, nullptr));
        ERROR_IF(numQueueGroups == 0, "No queue groups found!");
        std::vector<ze_command_queue_group_properties_t> queueProperties(numQueueGroups);
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetCommandQueueGroupProperties(device, &numQueueGroups, queueProperties.data()));
        uint32_t commandQueueOrdinalCompute{};
        uint32_t commandQueueOrdinalCopy{};
        uint32_t commandQueueOrdinalCopyOnly{};
        bool commandQueueHasCompute = false;
        bool commandQueueHasCopy = false;
        bool commandQueueHasCopyOnly = false;
        for (uint32_t i = 0; i < numQueueGroups; i++) {
            const bool isCompute = queueProperties[i].flags & ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE;
            const bool isCopy = queueProperties[i].flags & ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COPY;
            const bool isCopyOnly = !isCompute && isCopy;

            if (isCopyOnly) {
                commandQueueOrdinalCopyOnly = i;
                commandQueueHasCopyOnly = true;

                commandQueueOrdinalCopy = i;
                commandQueueHasCopy = true;
            }
            if (isCompute) {
                commandQueueOrdinalCompute = i;
                commandQueueHasCompute = true;
            }
            if (isCopy && !commandQueueHasCopyOnly) {
                commandQueueOrdinalCopy = i;
                commandQueueHasCopy = true;
            }
        }

        // Prepare queue descriptions
        if (commandQueueHasCompute) {
            this->commandQueueDescCompute = std::make_unique<ze_command_queue_desc_t>();
            this->commandQueueDescCompute->stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC;
            this->commandQueueDescCompute->mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS;
            this->commandQueueDescCompute->ordinal = commandQueueOrdinalCompute;
        }
        if (commandQueueHasCopy) {
            this->commandQueueDescCopy = std::make_unique<ze_command_queue_desc_t>();
            this->commandQueueDescCopy->stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC;
            this->commandQueueDescCopy->mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS;
            this->commandQueueDescCopy->ordinal = commandQueueOrdinalCopy;
        }
        if (commandQueueHasCopyOnly) {
            this->commandQueueDescCopyOnly = std::make_unique<ze_command_queue_desc_t>();
            this->commandQueueDescCopyOnly->stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC;
            this->commandQueueDescCopyOnly->mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS;
            this->commandQueueDescCopyOnly->ordinal = commandQueueOrdinalCopyOnly;
        }

        // Create queue
        if (createCommandQueue) {
            EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueCreate(context, device, commandQueueDescCompute.get(), &commandQueue));
        }
    }

    ~LevelZero() {
        if (commandQueue != nullptr) {
            EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueDestroy(commandQueue));
        }
        EXPECT_ZE_RESULT_SUCCESS(zeContextDestroy(context));
    }

    ze_command_queue_desc_t *getCommandQueueDescCopyOrNot(bool copy) {
        auto &result = copy ? commandQueueDescCopyOnly : commandQueueDescCompute;
        return result.get();
    }

    ze_driver_handle_t driver{};
    ze_device_handle_t device{};
    ze_context_handle_t context{};
    ze_command_queue_handle_t commandQueue{};
    ze_device_properties_t deviceProperties;

    std::unique_ptr<ze_command_queue_desc_t> commandQueueDescCompute{};
    std::unique_ptr<ze_command_queue_desc_t> commandQueueDescCopy{};
    std::unique_ptr<ze_command_queue_desc_t> commandQueueDescCopyOnly{};
};
