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

#include "framework/argument/abstract/argument.h"
#include "framework/utility/string_utils.h"

#include <vector>

struct StringArgument : Argument {
    using Argument::Argument;

    operator const std::string &() const {
        return value;
    }

    StringArgument &operator=(const std::string &newValue) {
        this->value = newValue;
        this->isValid = true;
        markAsParsed();
        return *this;
    }

    bool validate() const override {
        return isValid;
    }

  protected:
    std::string toStringValue() const override {
        return value;
    }

    void parseImpl(const std::string &valueToParse) override {
        this->value = valueToParse;
    }

    std::string value = {};
    bool isValid = false;
};
