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

#include "framework/argument/abstract/enum_argument.h"
#include "framework/enum/atomic_scope.h"

struct AtomicScopeArgument : EnumArgument<AtomicScopeArgument, AtomicScope> {
    using EnumArgument::EnumArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        markAsParsed();
        return *this;
    }

    const static inline std::string enumName = "atomic scope";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[2] = {EnumType::Workgroup, EnumType::Device};
    const static inline std::string enumValuesNames[2] = {"Workgroup", "Device"};
};
