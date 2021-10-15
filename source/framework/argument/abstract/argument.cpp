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

#include "argument.h"

#include "framework/argument/argument_container.h"
#include "framework/utility/command_line_argument.h"

#include <iomanip>

Argument::Argument(ArgumentContainer &parent, const std::string &key, const std::string &extraHelp)
    : key(key),
      extraHelp(extraHelp) {
    parent.pushArgument(*this);
}

const std::string Argument::getKey() const {
    return key;
}

std::string Argument::getHelp() const {
    std::ostringstream result;
    if (!extraHelp.empty()) {
        const int maxSize = 25;
        result << std::left << std::setw(maxSize);
    }

    result << getHelpEntry(key);
    if (!extraHelp.empty()) {
        result << extraHelp;
    }
    return result.str();
}

std::string Argument::getExtraHelp() const {
    return extraHelp;
}

void Argument::parse(CommandLineArgument &argument) {
    if (argument.isKeyEqualTo(this->key)) {
        argument.markAsProcessed();
        parseImpl(argument.getValue());
        markAsParsed();
    }
}

void Argument::markAsParsed() {
    this->parsed = true;
}

std::string Argument::getHelpEntry(const std::string &argumentKey) const {
    return std::string("--") + argumentKey + "=X ";
}
