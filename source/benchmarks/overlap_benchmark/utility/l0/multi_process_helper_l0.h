#pragma once

#include "framework/l0/levelzero.h"

#include "utility/multi_process_helper.h"

struct MultiProcessHelperL0 : MultiProcessHelper {
    static ze_result_t getSubDevicesForExecution(DeviceSelection subDevices,
                                                 size_t processesPerDevice,
                                                 ze_device_handle_t rootDevice,
                                                 std::vector<DeviceSelection> &outSubDevicesForExecution) {
        uint32_t actualSubDevicesCount = {};
        ZE_RESULT_SUCCESS_OR_RETURN(zeDeviceGetSubDevices(rootDevice, &actualSubDevicesCount, nullptr));
        outSubDevicesForExecution = MultiProcessHelper::getSubDevicesForExecution(subDevices, processesPerDevice, actualSubDevicesCount);
        return ZE_RESULT_SUCCESS;
    }
};
