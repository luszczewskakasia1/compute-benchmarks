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

#include "argument_container.h"

#include "framework/argument/abstract/argument.h"

#include <sstream>

void ArgumentContainer::pushArgument(Argument &argument) {
    this->arguments.push_back(&argument);
}

const std::vector<Argument *> &ArgumentContainer::getArguments() const {
    return arguments;
}

bool ArgumentContainer::parseArgument(CommandLineArgument &commandLineArgument) {
    for (auto &argument : arguments) {
        argument->parse(commandLineArgument);
    }
    return true;
}

bool ArgumentContainer::parseArguments(CommandLineArguments &commandLineArguments) {
    for (CommandLineArgument &commandLineArgument : commandLineArguments) {
        if (!this->parseArgument(commandLineArgument)) {
            return false;
        }
    }
    return true;
}

bool ArgumentContainer::validateArguments() const {
    for (const auto &argument : arguments) {
        if (!argument->validate()) {
            return false;
        }
    }

    if (!validateArgumentsExtra()) {
        return false;
    }

    return true;
}

std::string ArgumentContainer::getHelp(size_t indent) const {
    std::ostringstream result;
    for (const auto &argument : arguments) {
        for (auto i = 0u; i < indent; i++) {
            result << '\t';
        }
        result << argument->getHelp() << '\n';
    }
    return result.str();
}

std::vector<const Argument *> ArgumentContainer::getUnparsedArguments() const {
    std::vector<const Argument *> result = {};
    for (const auto &argument : arguments) {
        if (!argument->wasParsed()) {
            result.push_back(argument);
        }
    }
    return result;
}
