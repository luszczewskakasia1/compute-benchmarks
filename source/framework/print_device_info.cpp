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

#include "print_device_info.h"

#include "framework/configuration.h"
#include "framework/utility/error.h"

DeviceInfo::Functions DeviceInfo::functions[static_cast<int>(Api::COUNT)] = {};

void DeviceInfo::registerFunctions(Api api, PrintDeviceInfoFunction printDeviceInfo, PrintAvailableDevicesFunction printAvailableDevices) {
    FATAL_ERROR_IF(printDeviceInfo == nullptr, "Cannot register null function");
    FATAL_ERROR_IF(printAvailableDevices == nullptr, "Cannot register null function");

    auto &slot = functions[static_cast<int>(api)];
    FATAL_ERROR_IF(slot.printDeviceInfo != nullptr, "printDeviceInfo function registered multiple times");
    FATAL_ERROR_IF(slot.printAvailableDevices != nullptr, "printAvailableDevices function registered multiple times");
    slot.printDeviceInfo = printDeviceInfo;
    slot.printAvailableDevices = printAvailableDevices;
}

void DeviceInfo::printDeviceInfo() {
    for (int apiIndex = static_cast<int>(Api::FIRST); apiIndex <= static_cast<int>(Api::LAST); apiIndex++) {
        const Api api = static_cast<Api>(apiIndex);
        const Api selectedApi = Configuration::get().selectedApi;
        if (api != selectedApi && selectedApi != Api::All) {
            continue;
        }

        auto &printDeviceInfo = functions[static_cast<int>(api)].printDeviceInfo;

        if (printDeviceInfo == nullptr) {
            continue;
        }

        printDeviceInfo();
    }
}

void DeviceInfo::printAvailableDevices() {
    for (int apiIndex = static_cast<int>(Api::FIRST); apiIndex <= static_cast<int>(Api::LAST); apiIndex++) {
        const Api api = static_cast<Api>(apiIndex);
        const Api selectedApi = Configuration::get().selectedApi;
        if (api != selectedApi && selectedApi != Api::All) {
            continue;
        }

        auto &printAvailableDevices = functions[static_cast<int>(api)].printAvailableDevices;

        if (printAvailableDevices == nullptr) {
            continue;
        }

        printAvailableDevices();
    }
}
