#pragma once

#include "framework/configuration.h"
#include "framework/enum/device_selection.h"
#include "framework/enum/engine.h"
#include "framework/ocl/cl.h"
#include "framework/ocl/utility/queue_families_helper.h"

namespace OCL {
struct QueueProperties {
    bool createQueue = true;
    bool requireCreationSuccess = true;
    bool profiling = false;
    Engine selectedEngine = Engine::Unknown;
    bool useLegacyEngineSelection = false;
    int ooq = -1;
    DeviceSelection deviceSelection = DeviceSelection::Unknown;

    static QueueProperties create() {
        return QueueProperties()
            .setDeviceSelection(Configuration::get().subDeviceSelection);
    }

    QueueProperties &setProfiling(bool profiling) {
        this->profiling = profiling;
        return *this;
    }

    QueueProperties &setForceBlitter(bool bcs) {
        this->selectedEngine = bcs ? Engine::Bcs : Engine::Unknown;
        return *this;
    }

    QueueProperties &setForceEngine(Engine engine) {
        this->selectedEngine = engine;
        return *this;
    }

    QueueProperties &setUseLegacyEngineSelection(bool useLegacy) {
        this->useLegacyEngineSelection = useLegacy;
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
        FATAL_ERROR_UNLESS(DeviceSelectionHelper::hasSingleDevice(deviceSelection), "Queue can be created only on a single device");
        this->deviceSelection = deviceSelection;
        return *this;
    }

    bool fillQueueProperties(cl_device_id device, cl_queue_properties properties[], size_t size) const {
        std::fill_n(properties, size, 0);
        properties[0] = CL_QUEUE_PROPERTIES;
        cl_int propertiesIndex = 2;
        if (this->profiling) {
            properties[1] |= CL_QUEUE_PROFILING_ENABLE;
        }
        if (this->ooq == 1 || (this->ooq == -1 && Configuration::get().oclUseOOQ)) {
            properties[1] |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
        }
        if (this->selectedEngine != Engine::Unknown) {
            const auto propertiesForBlitter = QueueFamiliesHelper::getPropertiesForSelectingEngine(device, this->selectedEngine, this->useLegacyEngineSelection);
            if (propertiesForBlitter == nullptr) {
                return false;
            }

            for (auto i = 0u; i < propertiesForBlitter->propertiesCount; i++) {
                properties[propertiesIndex++] = propertiesForBlitter->properties[i];
            }
        }

        return true;
    }
};
} // namespace OCL
