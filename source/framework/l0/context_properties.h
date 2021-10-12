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

#include "framework/configuration.h"
#include "framework/enum/device_selection.h"

namespace L0 {
struct ContextProperties {
    DeviceSelection deviceSelection = DeviceSelection::Unknown;
    bool requireCreationSuccess = true;
    bool createContext = true;
    bool fakeSubDeviceAllowed = false;

    static ContextProperties create() {
        return ContextProperties()
            .setDeviceSelection(Configuration::get().subDeviceSelection);
    }

    ContextProperties &setDeviceSelection(DeviceSelection deviceSelection) {
        this->deviceSelection = deviceSelection;
        return *this;
    }

    ContextProperties &disable() {
        createContext = false;
        return *this;
    }

    ContextProperties &createSingleFakeSubDeviceIfNeeded() {
        this->fakeSubDeviceAllowed = true;
        return *this;
    }

    ContextProperties &allowCreationFail() {
        requireCreationSuccess = false;
        return *this;
    }
};
} // namespace L0
