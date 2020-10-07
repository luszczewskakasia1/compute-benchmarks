#pragma once

#include "framework/configuration.h"
#include "framework/enum/api.h"
#include "framework/test_case_argument/test_case_arguments.h"
#include "framework/utility/error.h"
#include "framework/utility/statistics.h"
#include "framework/utility/string_utils.h"

#include <functional>
#include <iostream>
#include <sstream>
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

        // Set if Intel extensions can be used from global configuration
        arguments.noIntelExtensions = ::configuration.noIntelExtensions;

        // Create statistics object
        const auto testCaseNameWithConfig = getTestCaseNameWithConfig(arguments);
        Statistics statistics{arguments.iterations, ::configuration.printType};

        // Validate arguments
        if (!arguments.validateArguments()) {
            statistics.printStatisticsString(testCaseNameWithConfig, "INVALID_ARGS");
            return;
        }

        // Get API
        const auto selectedApi = ::configuration.selectedApi; // Api selected by the user via the --api argument
        if (arguments.api != selectedApi && selectedApi != Api::All) {
            return;
        }

        // Get implementation
        const auto apiIndex = static_cast<int>(arguments.api);
        const auto &benchmarkImplementation = implementations[apiIndex];
        if (benchmarkImplementation.function == nullptr) {
            return;
        }

        // Silently skip benchmarks requiring Intel extensions if they were disabled
        if (arguments.noIntelExtensions && benchmarkImplementation.requiresIntelExtensions) {
            return;
        }

        // Run test
        const TestResult testResult = benchmarkImplementation.function(arguments, statistics);
        switch (testResult) {
        case TestResult::Success:
            ERROR_UNLESS(statistics.isFull(), "test did not generate as many values as expected");
            statistics.printStatistics(testCaseNameWithConfig);
            break;
        case TestResult::Error:
            statistics.printStatisticsString(testCaseNameWithConfig, "ERROR");
            break;
        case TestResult::DeviceNotCapable:
            ERROR_UNLESS(statistics.isEmpty(), "test was skipped but generated some values");
            break;
        case TestResult::DriverFunctionNotFound:
            ERROR_UNLESS(statistics.isEmpty(), "test was skipped but generated some values");
            statistics.printStatisticsString(testCaseNameWithConfig, "SKIPPED");
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

        return true;
    }

    std::string getTestCaseNameWithConfig(const Arguments &arguments) const {
        std::ostringstream result{};
        result << getTestCaseName() << "(api=" << std::to_string(arguments.api);

        const auto config = arguments.getCurrentConfig();
        if (config.size() > 0) {
            result << " " << config;
        }

        result << ")";
        return result.str();
    }
};
