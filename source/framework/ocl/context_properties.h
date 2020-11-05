#pragma once

#include "framework/configuration.h"
#include "framework/enum/device_selection.h"

struct ContextProperties {
    DeviceSelection deviceSelection = DeviceSelection::Unknown;
    bool requireCreationSuccess = true;

    static ContextProperties create() {
        return ContextProperties()
            .setDeviceSelection(::configuration.subDeviceSelection);
    }

    ContextProperties &setDeviceSelection(DeviceSelection deviceSelection) {
        this->deviceSelection = deviceSelection;
        return *this;
    }

    ContextProperties &allowCreationFail() {
        requireCreationSuccess = false;
        return *this;
    }
};
