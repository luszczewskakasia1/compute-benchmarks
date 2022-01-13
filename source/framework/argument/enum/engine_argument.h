/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2022 Intel Corporation
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
#include "framework/enum/engine.h"

struct EngineArgument : EnumArgument<EngineArgument, Engine> {
    using EnumArgument::EnumArgument;
    ThisType &operator=(EnumType newValue) {
        this->value = newValue;
        markAsParsed();
        return *this;
    }

    const static inline std::string enumName = "engine selection";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[14] = {EnumType::Rcs, EnumType::Ccs0, EnumType::Ccs1, EnumType::Ccs2, EnumType::Ccs3, EnumType::Bcs, EnumType::Bcs1, EnumType::Bcs2, EnumType::Bcs3, EnumType::Bcs4, EnumType::Bcs5, EnumType::Bcs6, EnumType::Bcs7, EnumType::Bcs8};
    const static inline std::string enumValuesNames[14] = {"Rcs", "Ccs0", "Ccs1", "Ccs2", "Ccs3", "Bcs", "Bcs1", "Bcs2", "Bcs3", "Bcs4", "Bcs5", "Bcs6", "Bcs7", "Bcs8"};
};
