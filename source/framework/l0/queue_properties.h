/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021-2022 Intel Corporation
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

#include "framework/enum/engine.h"

#include <level_zero/ze_api.h>

namespace L0 {
struct QueueProperties {
    bool createQueue = true;
    bool requireCreationSuccess = true;
    Engine selectedEngine = Engine::Ccs0;
    DeviceSelection deviceSelection = DeviceSelection::Unknown;

    static QueueProperties create() {
        return QueueProperties()
            .setDeviceSelection(Configuration::get().subDeviceSelection);
    }

    QueueProperties &allowCreationFail() {
        requireCreationSuccess = false;
        return *this;
    }

    QueueProperties &setForceBlitter(bool bcs) {
        this->selectedEngine = bcs ? Engine::Bcs : Engine::Ccs0;
        return *this;
    }

    QueueProperties &setForceEngine(Engine engine) {
        this->selectedEngine = engine;
        return *this;
    }

    QueueProperties &disable() {
        this->createQueue = false;
        return *this;
    }

    QueueProperties &setDeviceSelection(DeviceSelection newValue) {
        FATAL_ERROR_IF(DeviceSelectionHelper::hasHost(newValue), "Cannot create queue on host device");
        FATAL_ERROR_UNLESS(DeviceSelectionHelper::hasSingleDevice(newValue), "Queue can be created only on a single device");
        this->deviceSelection = newValue;
        return *this;
    }
};

} // namespace L0
