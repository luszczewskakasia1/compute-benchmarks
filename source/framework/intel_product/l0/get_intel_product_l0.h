#pragma once

#include "framework/intel_product/get_intel_product.h"
#include "framework/l0/levelzero.h"

inline IntelProduct getIntelProduct(ze_device_handle_t device) {
    ze_device_properties_t properties{};
    ze_result_t retVal = zeDeviceGetProperties(device, &properties);
    if (retVal != ZE_RESULT_SUCCESS) {
        return IntelProduct::Unknown;
    }
    return getIntelProduct(static_cast<uint32_t>(properties.deviceId));
}

inline IntelProduct getIntelProduct(const ze_device_properties_t &deviceProperties) {
    return getIntelProduct(deviceProperties.deviceId);
}

inline IntelProduct getIntelProduct(const LevelZero &levelzero) {
    return getIntelProduct(levelzero.getDeviceProperties());
}

template <typename Arg>
inline IntelGen getIntelGen(Arg &&arg) {
    return getIntelGen(getIntelProduct(std::forward<Arg>(arg)));
}
