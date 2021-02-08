#include "multi_process_helper.h"

#include <sstream>

std::vector<DeviceSelection> MultiProcessHelper::getSubDevicesForExecution(DeviceSelection subDevices, size_t processesPerDevice, size_t actualSubDevicesCount) {
    if (actualSubDevicesCount == 0) {
        actualSubDevicesCount = 1;
    }

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

std::string MultiProcessHelper::getProcessName(const std::vector<DeviceSelection> &subDevicesForExecution, size_t processIndex) {
    std::ostringstream processName{};
    processName << "process" << processIndex << " (" << DeviceSelectionHelper::toString(subDevicesForExecution[processIndex]) << ")";
    return processName.str();
}
