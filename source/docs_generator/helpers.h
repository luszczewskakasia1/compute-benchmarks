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

#include <sstream>
#include <string>
#include <vector>

void assignAndValidate(std::string &destination, const std::string &source, const char *message) {
    if (destination.empty()) {
        destination = source;
    } else {
        FATAL_ERROR_IF(destination != source, "String mismatch detected during ", message);
    }
}

std::pair<std::string, std::string> splitToTwoTokens(const std::string &line) {
    std::vector<std::string> result = {};
    std::istringstream stringStream(line);

    const size_t pos = line.find_first_of(';');
    FATAL_ERROR_IF(pos == std::string::npos, "Expected ';' delimetr in line");
    const std::string token0 = line.substr(0, pos);
    std::string token1 = line.substr(pos + 1);
    FATAL_ERROR_IF(token1.find(';') != std::string::npos, "Too many ';' delimeters in line");

    while (token1.size() > 0 && token1.back() == '\r') {
        token1.pop_back();
    }
    return {token0, token1};
}
