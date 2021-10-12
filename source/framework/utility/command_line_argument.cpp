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

#include "command_line_argument.h"

#include "framework/utility/error.h"
#include "framework/utility/string_utils.h"

#include <unordered_set>

CommandLineArgument::CommandLineArgument(const char *token) {
    this->valid = parseArgumentToKeyValue(token, this->key, this->value);
}

bool CommandLineArgument::parseArguments(int argc, char **argv, CommandLineArguments &outArguments, std::string &outErrorMessage) {
    std::unordered_set<std::string> allKeys = {};

    for (int argIndex = 1; argIndex < argc; argIndex++) {
        CommandLineArgument arg{argv[argIndex]};

        // Validate syntax
        if (!arg.isValid()) {
            outErrorMessage = std::string("Argument \"") + argv[argIndex] + "\" is ill-formed. All arguments have to follow syntax: --<key>[=value]";
            outArguments.clear();
            return false;
        }

        // Check for duplicates
        const auto [iterator, inserted] = allKeys.insert(arg.getKey());
        if (!inserted) {
            outErrorMessage = std::string("Argument with a key \"") + arg.getKey() + "\" is provided more than once";
            outArguments.clear();
            return false;
        }

        outArguments.push_back(std::move(arg));
    }

    return true;
}

std::vector<const CommandLineArgument *> CommandLineArgument::getUnprocessedArguments(const CommandLineArguments &arguments) {
    std::vector<const CommandLineArgument *> unprocessedArguments = {};
    for (const auto &argument : arguments) {
        if (!argument.isProcessed()) {
            unprocessedArguments.push_back(&argument);
        }
    }
    return unprocessedArguments;
}

void CommandLineArgument::markAsProcessed() {
    DEVELOPER_WARNING_IF(!this->valid, "Invalid argument was marked as processed");
    DEVELOPER_WARNING_IF(this->processed, "Argument processed multiple times")
    this->processed = true;
}

bool CommandLineArgument::isKeyEqualTo(const std::string &key) const {
    if (!valid) {
        return false;
    }
    return this->key == key;
}
