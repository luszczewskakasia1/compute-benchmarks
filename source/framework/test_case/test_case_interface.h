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

#include "framework/utility/command_line_argument.h"

#include <memory>
#include <string>

struct ArgumentContainer;

struct TestCaseInterface {
    virtual ~TestCaseInterface() = default;
    virtual bool runFromCommandLine(CommandLineArguments &commandLineArguments) = 0;
    virtual bool isApiImplemented(Api api) const = 0;
    virtual std::vector<Api> getApisWithImplementation() const = 0;
    virtual std::unique_ptr<ArgumentContainer> getArguments() const = 0;
    virtual std::string getHelp() const = 0;
    virtual std::string getHelpParameters() const = 0;
    virtual std::string getTestCaseName() const = 0;
};
