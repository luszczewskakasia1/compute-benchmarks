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

        // Create queue
        if (createCommandQueue) {
            uint32_t numQueueGroups = 0;
            EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetCommandQueueGroupProperties(device, &numQueueGroups, nullptr));
            if (numQueueGroups == 0) {
                ERROR("No queue groups found!");
            }

            std::vector<ze_command_queue_group_properties_t> queueProperties(numQueueGroups);
            EXPECT_ZE_RESULT_SUCCESS(zeDeviceGetCommandQueueGroupProperties(device, &numQueueGroups, queueProperties.data()));

            ze_command_queue_desc_t commandQueueDesc{ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC};
            for (uint32_t i = 0; i < numQueueGroups; i++) {
                if (queueProperties[i].flags & ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE) {
                    commandQueueDesc.ordinal = i;
                    break;
                }
            }
            commandQueueDesc.index = 0;
            commandQueueDesc.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS;
            EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueCreate(context, device, &commandQueueDesc, &commandQueue));

            this->commandListCommandQueueGroupOrdinal = commandQueueDesc.ordinal;
        }
    }

    ~LevelZero() {
        if (commandQueue != nullptr) {
            EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueDestroy(commandQueue));
        }
        EXPECT_ZE_RESULT_SUCCESS(zeContextDestroy(context));
    }

    ze_driver_handle_t driver{};
    ze_device_handle_t device{};
    ze_context_handle_t context{};
    ze_command_queue_handle_t commandQueue{};
    uint32_t commandListCommandQueueGroupOrdinal{};
    ze_device_properties_t deviceProperties;
};
