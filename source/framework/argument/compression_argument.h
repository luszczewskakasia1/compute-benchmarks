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

#include "framework/argument/basic_argument.h"
#include "framework/configuration.h"

struct CompressionBooleanArgument : BooleanArgument {
    using BooleanArgument::BooleanArgument;

    CompressionBooleanArgument &operator=(bool newValue) {
        this->value = newValue;
        markAsParsed();
        return *this;
    }

  protected:
    std::string toStringValue() const override {
        if (Configuration::get().noIntelExtensions) {
            return "?";
        }
        return BooleanArgument::toStringValue();
    }
};
