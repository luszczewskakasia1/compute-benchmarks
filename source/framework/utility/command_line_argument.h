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

#include <string>
#include <vector>

class CommandLineArgument {
  public:
    using CommandLineArguments = std::vector<CommandLineArgument>;

    CommandLineArgument(const char *token);
    CommandLineArgument(const CommandLineArgument &) = delete;
    CommandLineArgument &operator=(const CommandLineArgument &) = delete;
    CommandLineArgument(CommandLineArgument &&) = default;
    CommandLineArgument &operator=(CommandLineArgument &&) = default;

    static bool parseArguments(int argc, char **argv, CommandLineArguments &outArguments, std::string &outErrorMessage);
    static std::vector<const CommandLineArgument *> getUnprocessedArguments(const CommandLineArguments &arguments);

    void markAsProcessed();

    const std::string &getKey() const { return key; }
    const std::string &getValue() const { return value; }
    bool isKeyEqualTo(const std::string &key) const;
    bool isValid() const { return valid; }
    bool isProcessed() const { return processed; }

  private:
    std::string key = "";
    std::string value = "";
    bool valid = true;
    bool processed = false;
};

using CommandLineArguments = CommandLineArgument::CommandLineArguments;
