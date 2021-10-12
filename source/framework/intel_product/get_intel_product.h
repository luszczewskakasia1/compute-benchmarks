/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#pragma once

#include <cstdint>
#include <string>

// clang-format off
#define INTEL_GEN(GEN) GEN,
enum class IntelGen {
    #include "framework/intel_product/intel_products.inl"
};
#undef INTEL_GEN

#define INTEL_PRODUCT(PRODUCT, GEN) PRODUCT,
enum class IntelProduct {
     #include "framework/intel_product/intel_products.inl"
};
#undef INTEL_PRODUCT

#define INTEL_PRODUCT_ID(DEVICE_ID, PRODUCT) case DEVICE_ID: return IntelProduct::PRODUCT;
inline IntelProduct getIntelProduct(uint32_t deviceId) {
    switch (deviceId) {
        #include "framework/intel_product/intel_products.inl"
        default: return IntelProduct::Unknown;
    }
}
#undef INTEL_PRODUCT_ID

#define INTEL_PRODUCT(PRODUCT, GEN) case IntelProduct::PRODUCT: return IntelGen::GEN;
inline IntelGen getIntelGen(IntelProduct product) {
    switch (product) {
        #include "framework/intel_product/intel_products.inl"
        default: return IntelGen::Unknown;
    }
}
#undef INTEL_PRODUCT

template <typename Arg>
inline IntelGen getIntelGen(Arg &&arg) {
    return getIntelGen(getIntelProduct(std::forward<Arg>(arg)));
}

#define INTEL_PRODUCT(PRODUCT, GEN) case IntelProduct::PRODUCT: return #PRODUCT;
namespace std {
    inline std::string to_string(IntelProduct product) {
        switch (product) {
            #include "framework/intel_product/intel_products.inl"
            default: return "Unknown";
        }
    }
}
#undef INTEL_PRODUCT

#define INTEL_GEN(GEN) case IntelGen::GEN: return #GEN;
namespace std {
    inline std::string to_string(IntelGen gen) {
        switch (gen) {
            #include "framework/intel_product/intel_products.inl"
            default: return "Unknown";
        }
    }
}
#undef INTEL_GEN

// clang-format on
