/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#include "multi_process_helper.h"

#include <sstream>

std::vector<DeviceSelection> MultiProcessHelper::getSubDevicesForExecution(DeviceSelection subDevices, size_t processesPerDevice, size_t actualSubDevicesCount) {
    // Split bitfield of merged subDevices to a vector of single subDevices
    std::vector<DeviceSelection> subDevicesForExecution = DeviceSelectionHelper::split(subDevices);

    // Validate if we're not using an illegal subDevice
    for (DeviceSelection subDevice : subDevicesForExecution) {
        if (DeviceSelectionHelper::getSubDeviceIndex(subDevice) >= actualSubDevicesCount) {
            return {};
        }
    }

    // Duplicate all subDevices respectively if we have more than one processPerDevice
    const auto tilesForExecutionCount = subDevicesForExecution.size();
    for (auto i = 1u; i < processesPerDevice; i++) {
        for (auto j = 0u; j < tilesForExecutionCount; j++) {
            subDevicesForExecution.push_back(subDevicesForExecution[j]);
        }
    }

    return subDevicesForExecution;
}

std::string MultiProcessHelper::createAffinityMask(size_t rootDeviceIndex, DeviceSelection subDevices) {
    std::ostringstream result{};
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

std::string MultiProcessHelper::createProcessName(const std::vector<DeviceSelection> &subDevicesForExecution, size_t processIndex) {
    std::ostringstream processName{};
    processName << "process" << processIndex << " (" << DeviceSelectionHelper::toString(subDevicesForExecution[processIndex]) << ")";
    return processName.str();
}
