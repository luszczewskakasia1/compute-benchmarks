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

#include "framework/enum/api.h"
#include "framework/test_case/test_case_interface.h"

struct TestCaseArgumentContainer;

// This class implements test-agnostic functionality of the TestCase class. All methods, which do not require
// a concrete TestCaseArgument class for a specific test should be placed in this class as a protected method.
class TestCaseBase : public TestCaseInterface {
  protected:
    static bool parseArguments(TestCaseArgumentContainer &arguments, CommandLineArguments &commandLineArguments);
    std::vector<Api> getApisWithImplementation() const override;
    std::string getTestCaseNameWithConfig(const TestCaseArgumentContainer &arguments, bool commandLine) const;

    // Filters
    bool matchesWithTestFilter() const;
    bool matchesWithArgFilter(const ArgumentContainer &arguments) const;

    // Warnings
    void printTestMapWarning() const;
    void printTestCaseNameLengthWarning(const std::string &testCaseNameWithConfig) const;
};
