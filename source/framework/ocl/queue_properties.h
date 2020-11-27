#pragma once

#include "framework/configuration.h"
#include "framework/enum/device_selection.h"
#include "framework/ocl/cl.h"

#include <CL/intel/embargo/cl_ext_private_internal.h>

namespace OCL {
struct QueueProperties {
    bool createQueue = true;
    bool requireCreationSuccess = true;
    bool profiling = false;
    int copyQueue = -1;
    int ooq = -1;
    DeviceSelection deviceSelection = DeviceSelection::Unknown;

    static QueueProperties create() {
        return QueueProperties()
            .setDeviceSelection(::configuration.subDeviceSelection);
    }

    QueueProperties &setProfiling(bool profiling) {
        this->profiling = profiling;
        return *this;
    }

    QueueProperties &setBcs(bool bcs) {
        this->copyQueue = bcs;
        return *this;
    }

    QueueProperties &setOoq(bool ooq) {
        this->ooq = ooq;
        return *this;
    }

    QueueProperties &disable() {
        createQueue = false;
        return *this;
    }

    QueueProperties &allowCreationFail() {
        requireCreationSuccess = false;
        return *this;
    }

    QueueProperties &setDeviceSelection(DeviceSelection deviceSelection) {
        ERROR_UNLESS(DeviceSelectionHelper::hasSingleDevice(deviceSelection), "Queue can be created only on a single device");
        this->deviceSelection = deviceSelection;
        return *this;
    }
};
} // namespace OCL
