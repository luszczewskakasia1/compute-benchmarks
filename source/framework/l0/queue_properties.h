#pragma once

#include <level_zero/ze_api.h>

namespace L0 {
struct QueueProperties {
    bool createQueue = true;
    bool requireCreationSuccess = true;
    bool forceBlitter = false;
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
        this->forceBlitter = bcs;
        return *this;
    }

    QueueProperties &disable() {
        this->createQueue = false;
        return *this;
    }

    QueueProperties &setDeviceSelection(DeviceSelection deviceSelection) {
        ERROR_IF(DeviceSelectionHelper::hasHost(deviceSelection), "Canno create queue on host device");
        ERROR_UNLESS(DeviceSelectionHelper::hasSingleDevice(deviceSelection), "Queue can be created only on a single device");
        this->deviceSelection = deviceSelection;
        return *this;
    }
};

} // namespace L0
