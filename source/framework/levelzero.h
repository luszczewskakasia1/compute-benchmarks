#pragma once

#include <fstream>
#include <iostream>

#include <gtest/gtest.h>
#include <level_zero/ze_api.h>

#define ASSERT_ZE_RESULT_SUCCESS(retVal)      \
    if (retVal != ZE_RESULT_SUCCESS) {        \
        EXPECT_EQ(ZE_RESULT_SUCCESS, retVal); \
        return true;                          \
    }

#define EXPECT_ZE_RESULT_SUCCESS(retVal)      \
    if (retVal != ZE_RESULT_SUCCESS) {        \
        EXPECT_EQ(ZE_RESULT_SUCCESS, retVal); \
    }

inline const std::vector<const char *> &getResourcesSearchLocations() {
    static std::vector<const char *> locations {
        "test_files/spv_modules/",
    };
    return locations;
}

/* read binary file into a non-NULL-terminated string */
template <typename SizeT>
inline std::unique_ptr<char[]> readBinaryFile(const std::string &name, SizeT &outSize) {
    for (const char *base : getResourcesSearchLocations()) {
        std::string s(base);
        std::ifstream file(s + name, std::ios_base::in | std::ios_base::binary);
        if (false == file.good()) {
            continue;
        }

        size_t length;
        file.seekg(0, file.end);
        length = static_cast<size_t>(file.tellg());
        file.seekg(0, file.beg);

        auto storage = std::make_unique<char[]>(length);
        file.read(storage.get(), length);

        outSize = static_cast<SizeT>(length);
        return storage;
    }
    outSize = 0;
    return nullptr;
}

struct LevelZero {
    LevelZero() {
        EXPECT_ZE_RESULT_SUCCESS(zeInit(ZE_INIT_FLAG_NONE));

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

        ze_command_queue_desc_t commandQueueDesc = {ZE_COMMAND_QUEUE_DESC_VERSION_CURRENT};
        commandQueueDesc.ordinal = 0;
        commandQueueDesc.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS;
        EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueCreate(device, &commandQueueDesc, &commandQueue));
    }

    ~LevelZero() {
        EXPECT_ZE_RESULT_SUCCESS(zeCommandQueueDestroy(commandQueue));
    }

    ze_driver_handle_t driver;
    ze_device_handle_t device;
    ze_command_queue_handle_t commandQueue;
};
