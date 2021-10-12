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

enum class AtomicMemoryOrder {
    Unknown,
    Relaxed,
    Acquire,
    Release,
    AcquireRelease,
    SequentialConsitent,
};

namespace AtomicMemoryOrderHelper {
constexpr inline std::array<AtomicMemoryOrder, 5> allValues = {
    AtomicMemoryOrder::Relaxed,
    AtomicMemoryOrder::Acquire,
    AtomicMemoryOrder::Release,
    AtomicMemoryOrder::AcquireRelease,
    AtomicMemoryOrder::SequentialConsitent,
};

inline std::string toOpenclC(AtomicMemoryOrder order) {
    switch (order) {
    case AtomicMemoryOrder::Relaxed:
        return "memory_order_relaxed";
    case AtomicMemoryOrder::Acquire:
        return "memory_order_acquire";
    case AtomicMemoryOrder::Release:
        return "memory_order_release";
    case AtomicMemoryOrder::AcquireRelease:
        return "memory_order_acq_rel";
    case AtomicMemoryOrder::SequentialConsitent:
        return "memory_order_seq_cst";
    default:
        FATAL_ERROR("Unknown memory order");
    }
}
} // namespace AtomicMemoryOrderHelper
