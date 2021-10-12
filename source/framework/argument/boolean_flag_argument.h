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

struct BooleanFlagArgument : Argument {
    using Argument::Argument;

    operator bool() const {
        return value;
    }

    BooleanFlagArgument &operator=(bool value) {
        this->value = value;
        markAsParsed();
        return *this;
    }

    bool validate() const override {
        return isValid;
    }

  protected:
    std::string toStringValue() const override {
        return std::to_string(this->value);
    }

    void parseImpl(const std::string &value) override {
        this->value = true;
        this->isValid = value.empty();
    }

    std::string getHelpEntry(const std::string &key) const override {
        return std::string("--") + key;
    }

    bool value = false;
    bool isValid = true;
};
