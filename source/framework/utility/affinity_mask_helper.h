#pragma once

#include "framework/enum/device_selection.h"

#include <sstream>
#include <string>

struct AffinityMaskHelper {
    static std::string createAffinityMask(size_t rootDeviceIndex, DeviceSelection subDevices) {
        std::ostringstream result = {};
        const auto subDevicesSplit = DeviceSelectionHelper::split(subDevices);
        for (int i = 0; i < subDevicesSplit.size(); i++) {
            const auto subDeviceIndex = DeviceSelectionHelper::getSubDeviceIndex(subDevicesSplit[i]);
            result << rootDeviceIndex << '.' << subDeviceIndex;
            if (i != subDevicesSplit.size() - 1) {
                result << ",";
            }
        }
        return result.str();
    }
};
