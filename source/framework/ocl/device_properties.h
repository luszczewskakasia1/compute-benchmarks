#pragma once

#include "framework/configuration.h"
#include "framework/enum/device_selection.h"
#include "framework/ocl/cl.h"

struct DeviceProperties {
    DeviceSelection subDeviceSelection = DeviceSelection::Unknown;
    bool requireSubDeviceCreationSuccess = true;

    static DeviceProperties create() {
        return DeviceProperties()
            .setDeviceSelection(::configuration.subDeviceSelection);
    }

    DeviceProperties &setDeviceSelection(DeviceSelection deviceSelection) {
        this->subDeviceSelection = deviceSelection;
        return *this;
    }

    DeviceProperties &allowSubDeviceCreationFail() {
        requireSubDeviceCreationSuccess = false;
        return *this;
    }
};
