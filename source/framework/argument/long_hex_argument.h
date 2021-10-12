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
#include "framework/utility/hex_helper.h"

struct LongHexArgument : Argument {
    using Argument::Argument;

    operator const std::vector<uint8_t> &() const {
        return bytes;
    }

    LongHexArgument &operator=(const std::string &hex) {
        parseImpl(hex);
        markAsParsed();
        return *this;
    }

    bool validate() const override {
        return bytes.size() > 0;
    }

  protected:
    std::string toStringValue() const override {
        return HexHelper::toHex(bytes);
    }

    void parseImpl(const std::string &value) override {
        bytes = HexHelper::fromHex(value);
    }

    std::vector<uint8_t> bytes;
};
