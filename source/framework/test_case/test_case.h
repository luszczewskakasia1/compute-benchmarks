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
    Nooped,                  // Test was nooped, only print its name
    FilteredOut,             // Test was skipped because of passed argFilter
    VerificationFail,        // Results where incorrect
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

    bool runFromCommandLine(CommandLineArguments &commandLineArguments) override {
        // Parse test-specific parameters
        Arguments arguments;
        if (!parseArguments(arguments, commandLineArguments)) {
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
        const auto testResult = runImpl(statistics, arguments, testCaseNameWithConfig);
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
        case TestResult::FilteredOut:
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

        case TestResult::Nooped:
            statistics.printStatisticsString(testCaseNameWithConfig, "NOOP");
            break;

        case TestResult::VerificationFail:
            statistics.printStatisticsString(testCaseNameWithConfig, "VERIF_FAIL");
            break;

        default:
            ERROR("unknown result was returned by test");
        }
    }

  private:
    TestResult runImpl(Statistics &statistics, const Arguments &arguments, const std::string &testCaseNameWithConfig) const {
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

        // Check test filters
        if (auto testFilters = ::configuration.testFilter.get(); testFilters.size() > 0) {
            const auto testCaseName = getTestCaseName();
            const auto matches = [&](const std::string &testFilter) { return testFilter == testCaseName; };
            const auto requirementMet = std::any_of(testFilters.begin(), testFilters.end(), matches);
            if (!requirementMet) {
                return TestResult::FilteredOut;
            }
        }

        // Check arg filters
        for (const std::string &argFilter : ::configuration.argFilter.get()) {
            const std::vector<TestCaseArgument *> &args = arguments.arguments;
            const auto matches = [&](TestCaseArgument *arg) { return arg->toString() == argFilter; };
            const bool requirementMet = std::any_of(args.begin(), args.end(), matches);
            if (!requirementMet) {
                return TestResult::FilteredOut;
            }
        }

        // Check if test case name with config is not too long
        if (!::configuration.dumpCommandLines && !arguments.isSingleTestMode) {
            printTestCaseNameLengthWarning(testCaseNameWithConfig);
        }

        // Noop if required
        if (::configuration.noop) {
            return TestResult::Nooped;
        }

        // Run the test
        return benchmarkImplementation.function(arguments, statistics);
    }

    static bool parseArguments(Arguments &arguments, CommandLineArguments &commandLineArguments) {
        arguments.isSingleTestMode = true;
        for (auto &commandLineArgument : commandLineArguments) {
            if (!arguments.parseArgument(commandLineArgument)) {
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

    void printTestCaseNameLengthWarning(const std::string &testCaseNameWithConfig) const {
        const static size_t columnWidth = getTestCaseNameColumnWidth();
        static size_t maxWidth = columnWidth;

        const size_t currentWidth = testCaseNameWithConfig.length();
        if (currentWidth > maxWidth) {
            maxWidth = currentWidth;
            std::cerr << "WARNING: current TestCase column width of " << columnWidth << " is too small. Consider changing it to " << maxWidth << ". "
                      << "This is an issue in the benchmark which may cause the output to appear weird, but does not break any functionality.\n";
        }
    }
};
