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
#include "framework/utility/error.h"

#include <array>

enum class AtomicScope {
    Unknown,
    Workgroup,
    Device,
};

namespace AtomicScopeHelper {
constexpr inline std::array<AtomicScope, 2> allValues = {
    AtomicScope::Workgroup,
    AtomicScope::Device,
};

inline std::string toOpenclC(AtomicScope order) {
    switch (order) {
    case AtomicScope::Workgroup:
        return "memory_scope_work_group";
    case AtomicScope::Device:
        return "memory_scope_device";
    default:
        FATAL_ERROR("Unknown memory scope");
    }
}
} // namespace AtomicScopeHelper
