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
#include <unordered_map>

enum class TestResult {
    Success,                 // should be returned after a successful run
    Error,                   // an error was returned by the compute API
    DriverFunctionNotFound,  // extension function was not found and test is skipped
    DeviceNotCapable,        // device does not support some functionality needed in test (e.g. compression)
    KernelNotFound,          // binary kernel was not found in working directory
    SkippedApi,              // selected API should not be run, because of user has disabled it with a command-line argument
    UnsupportedApi,          // selected API should not be run, because it is not built in current binary
    NoImplementation,        // Test is not implemented in current API
    IntelExtensionsRequired, // Intel extensions are required, but they are disabled
    InvalidArgs,             // Invalid arguments specific to the test case were supplied
    Nooped,                  // Test was nooped, only print its name
    FilteredOut,             // Test was skipped because of passed argFilter
    VerificationFail,        // Results where incorrect
    KernelBuildError         // Kernel could not be compiled
};

struct TestResultHelper {
    struct TestResultInfo {
        const std::string stringMessage;
        const bool printInSingleTestMode;
        const bool printInAllTestsMode;
        const bool wasTestSkipped;
    };

    static const TestResultInfo &getTestResultInfo(TestResult testResult);
};
