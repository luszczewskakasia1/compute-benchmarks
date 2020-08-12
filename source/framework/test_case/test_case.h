#pragma once

#include "framework/api.h"
#include "framework/configuration.h"
#include "framework/error.h"
#include "framework/statistics.h"
#include "framework/string_utils.h"
#include "framework/test_case/test_case_arguments.h"

#include <functional>
#include <iostream>
#include <type_traits>

struct TestCaseInterface {
    virtual bool runFromCommandLine(int argc, char **argv) = 0;
    virtual std::string getHelp() const = 0;
    virtual std::string getHelpParameters() const = 0;
    virtual std::string getTestCaseName() const = 0;
};

enum class TestResult {
    Success,                // should be returned after a successful run
    Error,                  // an error was returned by the compute API
    DriverFunctionNotFound, // extension function was not found and test is skipped
    DeviceNotCapable,       // device does not support some functionality needed in test (e.g. compression)
    KernelNotFound,         // binary kernel was not found in working directory
};

template <typename Arguments>
class TestCase : public TestCaseInterface {
    static_assert(std::is_base_of_v<TestCaseArguments, Arguments>, "Arguments class should derive from TestCaseArguments");

  public:
    using BenchmarkImplementation = std::function<TestResult(Arguments, Statistics &)>;
    static inline BenchmarkImplementation implementations[(int)Api::COUNT];

    std::string getHelpParameters() const override { return Arguments{}.getHelp(); }

    bool runFromCommandLine(int argc, char **argv) override {
        // Parse test-specific parameters
        Arguments arguments;
        if (!parseArguments(arguments, argc, argv)) {
            return false;
        }

        // Try running with all possible APIs. If some are disabled, e.g. --api=ocl is passed, then the rest will be skipped in run() method
        for (int apiIndex = 0; apiIndex < static_cast<int>(Api::COUNT); apiIndex++) {
            arguments.api = static_cast<Api>(apiIndex);
            run(arguments);
        }
        return true;
    }

    void run(Arguments arguments) const {
        // Set iterations count from global configuration
        if (arguments.iterations != 0) {
            std::cerr << "WARNING: arguments.iterations was not zero. Overriding with value from global configuration - "
                      << ::configuration.iterations << ".\n";
        }
        arguments.iterations = ::configuration.iterations;

        // Get API
        const auto apiIndex = static_cast<int>(arguments.api);
        if (apiIndex >= static_cast<int>(Api::COUNT)) {
            std::cerr << "WARNING: unknown API selected. Test was skipped.\n";
            return;
        }
        const auto selectedApi = ::configuration.selectedApi; // Api selected by the user via the --api argument
        if (arguments.api != selectedApi && selectedApi != Api::All) {
            return;
        }

        // Get implementation
        const auto benchmarkImplementation = implementations[apiIndex];
        if (benchmarkImplementation == nullptr) {
            return;
        }

        // Get test case name
        const auto apiString = std::string{arguments.api == Api::OpenCL ? "api=ocl" : "api=l0"};
        const auto currentConfig = arguments.getCurrentConfig();
        const auto configWithApi = currentConfig.size() == 0 ? apiString : apiString + " " + currentConfig;
        const auto testCaseNameWithConfig = getTestCaseName() + "(" + configWithApi + ")";

        // Run test
        Statistics statistics{::configuration.iterations};
        const TestResult testResult = benchmarkImplementation(arguments, statistics);
        switch (testResult) {
        case TestResult::Success:
            ERROR_UNLESS(statistics.isFull(), "test did not generate as many values as expected");
            statistics.printStatistics(testCaseNameWithConfig, ::configuration.printType);
            break;
        case TestResult::Error:
            statistics.printStatisticsString(testCaseNameWithConfig, ::configuration.printType, "ERROR");
            break;
        case TestResult::DeviceNotCapable:
        case TestResult::DriverFunctionNotFound:
            ERROR_UNLESS(statistics.isEmpty(), "test was skipped but generated some values");
            statistics.printStatisticsString(testCaseNameWithConfig, ::configuration.printType, "SKIPPED");
            break;
        case TestResult::KernelNotFound:
            ERROR("binary kernel was not found. Kernels should be located in working directory");
        default:
            ERROR("unknown result was returned by test");
        }
    }

  private:
    static bool parseArguments(Arguments &arguments, int argc, char **argv) {
        for (int i = 2; i < argc; i++) {
            const auto argument = std::string{argv[i]};
            std::string key, value;
            if (!parseArgumentToKeyValue(argument, key, value)) {
                return false;
            }

            if (!arguments.parseArgument(key, value)) {
                return false;
            }
        }

        if (!arguments.validateArguments()) {
            return false;
        }

        return true;
    }
};
