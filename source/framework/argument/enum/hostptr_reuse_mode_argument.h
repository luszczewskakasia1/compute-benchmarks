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
#include "framework/enum/hostptr_reuse_mode.h"

struct HostptrBufferReuseModeArgument : EnumArgument<HostptrBufferReuseModeArgument, HostptrReuseMode> {
    using EnumArgument::EnumArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        markAsParsed();
        return *this;
    }

    const static inline std::string enumName = "Buffer hostptr reuse mode";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[3] = {EnumType::None, EnumType::Usm, EnumType::Map};
    const static inline std::string enumValuesNames[3] = {"None", "Usm", "Map"};
};

struct HostptrImageReuseModeArgument : EnumArgument<HostptrImageReuseModeArgument, HostptrReuseMode> {
    using EnumArgument::EnumArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        markAsParsed();
        return *this;
    }

    const static inline std::string enumName = "Image hostptr reuse mode";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[2] = {EnumType::None, EnumType::Map};
    const static inline std::string enumValuesNames[2] = {"None", "Map"};
};
