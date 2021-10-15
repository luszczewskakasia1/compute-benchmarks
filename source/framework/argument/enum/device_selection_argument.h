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
#include "framework/enum/device_selection.h"

struct DeviceSelectionArgument : EnumArgument<DeviceSelectionArgument, DeviceSelection> {
    using EnumArgument::EnumArgument;
    ThisType &operator=(EnumType newValue) {
        this->value = newValue;
        markAsParsed();
        return *this;
    }

    const static inline std::string enumName = "device selection";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[5] = {EnumType::Root, EnumType::Tile0, EnumType::Tile1, EnumType::Tile2, EnumType::Tile3};
    const static inline std::string enumValuesNames[5] = {"Root", "Tile0", "Tile1", "Tile2", "Tile3"};
};
