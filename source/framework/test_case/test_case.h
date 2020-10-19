#pragma once

#include "framework/benchmark_info.h"
#include "framework/configuration.h"
#include "framework/enum/api.h"
#include "framework/test_case/test_case_interface.h"
#include "framework/utility/error.h"
#include "framework/utility/statistics.h"
#include "framework/utility/string_utils.h"

#include <functional>
#include <iostream>
#include <sstream>
#include <type_traits>

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
};

template <typename _Arguments>
class TestCase : public TestCaseInterface {
  public:
    using Arguments = _Arguments;
    static_assert(std::is_base_of_v<TestCaseArguments, Arguments>, "Arguments class should derive from TestCaseArguments");

    struct BenchmarkImplementation {
        using Function = std::function<TestResult(Arguments, Statistics &)>;
        Function function = {};
        bool requiresIntelExtensions = false;
    };
    static inline BenchmarkImplementation implementations[(int)Api::COUNT];

    std::string getHelpParameters() const override { return Arguments{}.getHelp(2u); }

    bool runFromCommandLine(int argc, char **argv) override {
        // Parse test-specific parameters
        Arguments arguments;
        if (!parseArguments(arguments, argc, argv)) {
            return false;
        }

        // Try running with all possible APIs. If some are disabled, e.g. --api=ocl is passed, then the rest will be skipped in run() method
        for (int apiIndex = static_cast<int>(Api::FIRST); apiIndex <= static_cast<int>(Api::LAST); apiIndex++) {
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
        arguments.noIntelExtensions = ::configuration.noIntelExtensions;

        // Create statistics object
        const auto testCaseNameWithConfig = getTestCaseNameWithConfig(arguments, ::configuration.dumpCommandLines);
        Statistics statistics{arguments.iterations, ::configuration.printType};

        // Run test
        const auto testResult = runImpl(statistics, arguments);
        switch (testResult) {
        case TestResult::Success:
            ERROR_UNLESS(statistics.isFull(), "test did not generate as many values as expected");
            statistics.printStatistics(testCaseNameWithConfig);
            break;

        case TestResult::Error:
            statistics.printStatisticsString(testCaseNameWithConfig, "ERROR");
            break;

        case TestResult::InvalidArgs:
            statistics.printStatisticsString(testCaseNameWithConfig, "INVALID_ARGS");
            break;

        case TestResult::SkippedApi:
        case TestResult::NoImplementation:
        case TestResult::DeviceNotCapable:
        case TestResult::IntelExtensionsRequired:
            ERROR_UNLESS(statistics.isEmpty(), "test was skipped but generated some values");
            break;

        case TestResult::DriverFunctionNotFound:
            ERROR_UNLESS(statistics.isEmpty(), "test was skipped but generated some values");
            statistics.printStatisticsString(testCaseNameWithConfig, "SKIPPED");
            break;

        case TestResult::KernelNotFound:
            ERROR_UNLESS(statistics.isEmpty(), "test was skipped but generated some values");
            statistics.printStatisticsString(testCaseNameWithConfig, "MISSING_KERNEL");
            break;

        default:
            ERROR("unknown result was returned by test");
        }
    }

  private:
    TestResult runImpl(Statistics &statistics, const Arguments &arguments) const {
        // Get API
        const auto selectedApi = ::configuration.selectedApi;
        if (arguments.api != selectedApi && selectedApi != Api::All) {
            return TestResult::SkippedApi;
        }

        // Get implementation
        const auto apiIndex = static_cast<int>(arguments.api);
        const auto &benchmarkImplementation = implementations[apiIndex];
        if (benchmarkImplementation.function == nullptr) {
            return TestResult::NoImplementation;
        }

        // Check if test needs Intel extensions
        if (arguments.noIntelExtensions && benchmarkImplementation.requiresIntelExtensions) {
            return TestResult::IntelExtensionsRequired;
        }

        // Validate arguments
        if (!arguments.validateArguments()) {
            return TestResult::InvalidArgs;
        }

        // Verify if current test case is added to the test map
        if (getTestMap().find(getTestCaseName()) == getTestMap().end()) {
            printTestMapWarning();
        }

        // Run the test
        return benchmarkImplementation.function(arguments, statistics);
    }

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

        return true;
    }

    std::string getTestCaseNameWithConfig(const Arguments &arguments, bool commandLine) const {
        std::ostringstream result{};

        if (commandLine) {
            result << "--test=" << getTestCaseName() << " --api=" << std::to_string(arguments.api);
        } else {
            result << getTestCaseName() << "(api=" << std::to_string(arguments.api);
        }

        const auto config = arguments.getCurrentConfig(commandLine);
        if (config.size() > 0) {
            result << " " << config;
        }

        if (!commandLine) {
            result << ")";
        }

        return result.str();
    }

    void printTestMapWarning() const {
        static bool printed = false;
        if (printed) {
            return;
        }
        printed = true;

        std::cerr << "WARNING: \"" << getTestCaseName() << "\" is not added to the test map. This is an issue in the benchmark causing single-test mode to not work.\n";
    }
};
