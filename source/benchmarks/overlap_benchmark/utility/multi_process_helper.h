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

#pragma once

#include "framework/enum/device_selection.h"

struct MultiProcessHelper {
    constexpr static size_t workloadOperationsCount = 500000;
    constexpr static size_t workloadWorkgroupSize = 256;
    static std::vector<DeviceSelection> getSubDevicesForExecution(DeviceSelection subDevices, size_t processesPerDevice, size_t actualSubDevicesCount);
    static std::string createAffinityMask(size_t rootDeviceIndex, DeviceSelection subDevices);
    static std::string createProcessName(const std::vector<DeviceSelection> &subDevicesForExecution, size_t processIndex);
};
