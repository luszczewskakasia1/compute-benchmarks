#pragma once

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

struct LevelZero {
    LevelZero() : LevelZero(true) {}
    LevelZero(bool createCommandQueue) {
        EXPECT_ZE_RESULT_SUCCESS(zeInit(ZE_INIT_FLAG_GPU_ONLY));

        uint32_t driverCount = 0;
        EXPECT_ZE_RESULT_SUCCESS(zeDriverGet(&driverCount, nullptr));
        EXPECT_LT(0u, driverCount);
        driverCount = 1;
        EXPECT_ZE_RESULT_SUCCESS(zeDriverGet(&driverCount, &driver));

        uint32_t deviceCount = 0;
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGet(driver, &deviceCount, nullptr));
        EXPECT_LT(0u, deviceCount);
        deviceCount = 1;
        EXPECT_ZE_RESULT_SUCCESS(zeDeviceGet(driver, &deviceCount, &device));

        const ze_context_desc_t contextDesc{ZE_STRUCTURE_TYPE_CONTEXT_DESC};
        EXPECT_ZE_RESULT_SUCCESS(zeContextCreate(driver, &contextDesc, &context));

        if (createCommandQueue) {
            ze_command_queue_desc_t commandQueueDesc{ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC};
            commandQueueDesc.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS;
            EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueCreate(context, device, &commandQueueDesc, &commandQueue));
        }
    }

    ~LevelZero() {
        if (commandQueue != nullptr) {
            EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueDestroy(commandQueue));
        }
    }

    ze_driver_handle_t driver{};
    ze_device_handle_t device{};
    ze_context_handle_t context{};
    ze_command_queue_handle_t commandQueue{};
};
