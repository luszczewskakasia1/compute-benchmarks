/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021-2022 Intel Corporation
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

#include "framework/enum/api.h"
#include "framework/utility/command_line_argument.h"

#include <string>
#include <vector>

struct Argument;

struct ArgumentContainer {
    void pushArgument(Argument &argument);
    const std::vector<Argument *> &getArguments() const;

    bool parseArgument(CommandLineArgument &commandLineArgument);
    bool parseArguments(CommandLineArguments &commandLineArguments);
    virtual bool validateArguments() const;
    std::string getHelp(size_t indent) const;
    std::vector<const Argument *> getUnparsedArguments() const;

    virtual ~ArgumentContainer() = default;

  protected:
    std::vector<Argument *> arguments;
    virtual bool validateArgumentsExtra() const { return true; } // This is optional. Use this for validating dependencies between arguments if any.
};
