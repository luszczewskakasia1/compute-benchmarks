#pragma once

#include "framework/configuration.h"
#include "framework/enum/device_selection.h"
#include "framework/ocl/cl.h"

#include <CL/intel/embargo/cl_ext_private_internal.h>

namespace OCL {
struct QueueProperties {
    cl_command_queue_properties properties[5] = {CL_QUEUE_PROPERTIES, 0, 0, 0, 0};
    bool createQueue = true;
    bool requireCreationSuccess = true;
    DeviceSelection deviceSelection = DeviceSelection::Unknown;

    operator const cl_command_queue_properties *() const {
        return &properties[0];
    }

    static QueueProperties create() {
        return QueueProperties()
            .setProfiling(false)
            .setBcs(false)
            .setOoq(-1)
            .setDeviceSelection(::configuration.subDeviceSelection);
    }

    QueueProperties &setProfiling(bool profiling) {
        if (profiling) {
            properties[1] |= CL_QUEUE_PROFILING_ENABLE;
        }
        return *this;
    }

    QueueProperties &setBcs(bool bcs) {
        if (bcs) {
            properties[2] = CL_QUEUE_FAMILY_INTEL;
            properties[3] = CL_QUEUE_FAMILY_TYPE_BCS_INTEL;
        }
        return *this;
    }

    QueueProperties &setOoq(int ooq) {
        if (ooq == 1 || (ooq == -1 && ::configuration.oclUseOOQ)) {
            properties[1] |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
        }
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
