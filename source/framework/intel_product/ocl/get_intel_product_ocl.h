#pragma once

#include "framework/intel_product/get_intel_product.h"
#include "framework/ocl/opencl.h"
#ifdef USE_PCIACCESS
#include <pciaccess/pciaccess.h>
#endif


inline IntelProduct getIntelProduct(cl_device_id device) {
#ifdef USE_PCIACCESS
    pci_system_init();
#endif
    return IntelProduct::Unknown;
}

inline IntelProduct getIntelProduct(const Opencl &opencl) {
    return getIntelProduct(opencl.device);
}

template <typename Arg>
inline IntelGen getIntelGen(Arg &&arg) {
    return getIntelGen(getIntelProduct(std::forward<Arg>(arg)));
}
