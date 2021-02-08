#pragma once

#include "framework/enum/device_selection.h"

struct MultiProcessHelper {
    static std::vector<DeviceSelection> getSubDevicesForExecution(DeviceSelection subDevices, size_t processesPerDevice, size_t actualSubDevicesCount);
    static std::string createAffinityMask(size_t rootDeviceIndex, DeviceSelection subDevices);
    static std::string createProcessName(const std::vector<DeviceSelection> &subDevicesForExecution, size_t processIndex);
};
