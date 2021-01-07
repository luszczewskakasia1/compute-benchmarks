#pragma once

#include <unordered_map>
#include <string>

enum class TestResult {
    Success,                 // should be returned after a successful run
    Error,                   // an error was returned by the compute API
    DriverFunctionNotFound,  // extension function was not found and test is skipped
    DeviceNotCapable,        // device does not support some functionality needed in test (e.g. compression)
    KernelNotFound,          // binary kernel was not found in working directory
    SkippedApi,              // selected API should not be run
    NoImplementation,        // Test is not implemented in current API
    IntelExtensionsRequired, // Intel extensions are required, but they are disabled
    InvalidArgs,             // Invalid arguments specific to the test case were supplied
    Nooped,                  // Test was nooped, only print its name
    FilteredOut,             // Test was skipped because of passed argFilter
    VerificationFail,        // Results where incorrect
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
