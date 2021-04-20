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

    QueueProperties &setDeviceSelection(DeviceSelection deviceSelection) {
        FATAL_ERROR_IF(DeviceSelectionHelper::hasHost(deviceSelection), "Canno create queue on host device");
        FATAL_ERROR_UNLESS(DeviceSelectionHelper::hasSingleDevice(deviceSelection), "Queue can be created only on a single device");
        this->deviceSelection = deviceSelection;
        return *this;
    }
};

} // namespace L0
