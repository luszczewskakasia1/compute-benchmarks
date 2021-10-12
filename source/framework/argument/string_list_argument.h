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

struct StringListArgument : Argument {
    using Argument::Argument;

    operator const std::vector<std::string> &() const {
        return get();
    }

    const std::vector<std::string> &get() const {
        return value;
    }

    StringListArgument &operator=(const std::vector<std::string> &value) {
        this->value = value;
        this->isValid = true;
        markAsParsed();
        return *this;
    }

    bool validate() const override {
        return isValid;
    }

  protected:
    std::string toStringValue() const override {
        FATAL_ERROR("ArgFilterArgument should not be printed");
    }

    void parseImpl(const std::string &value) override {
        for (const auto &filter : splitString(value)) {
            this->value.push_back(filter);
        }
    }

    std::vector<std::string> value = {};
    bool isValid = false;
};
