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

enum class DataType {
    Unknown,
    Int32,
    Float,
};

namespace DataTypeHelper {
constexpr inline size_t getSize(DataType dataType) {
    switch (dataType) {
    case DataType::Int32:
        return 4u;
    case DataType::Float:
        return sizeof(float);
    default:
        FATAL_ERROR("Unkown data type")
    }
}

inline std::string toOpenclC(DataType dataType) {
    switch (dataType) {
    case DataType::Float:
        return "float";
    case DataType::Int32:
        return "int";
    default:
        FATAL_ERROR("Unknown data type");
    }
}

inline std::string toExplicitAtomicOpenclC(DataType dataType) {
    switch (dataType) {
    case DataType::Float:
        return "atomic_float";
    case DataType::Int32:
        return "atomic_int";
    default:
        FATAL_ERROR("Unknown data type");
    }
}
} // namespace DataTypeHelper
