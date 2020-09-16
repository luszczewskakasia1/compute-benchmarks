#pragma once

#include "framework/intel_product/get_intel_product.h"
#include "framework/ocl/opencl.h"

inline IntelProduct getIntelProduct(cl_device_id device) {
    return IntelProduct::Unknown;
}

inline IntelProduct getIntelProduct(const Opencl &opencl) {
    return getIntelProduct(opencl.device);
}

template <typename Arg>
inline IntelGen getIntelGen(Arg &&arg) {
    return getIntelGen(getIntelProduct(std::forward<Arg>(arg)));
}
