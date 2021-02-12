#pragma once

#include "framework/benchmark_info.h"
#include "framework/configuration.h"
#include "framework/enum/api.h"
#include "framework/test_case/test_case_interface.h"
#include "framework/test_case/test_case_statistics.h"
#include "framework/test_case/test_result.h"
#include "framework/test_map.h"
#include "framework/utility/common_help_message.h"
#include "framework/utility/error.h"
#include "framework/utility/string_utils.h"

#include <functional>
#include <iostream>
#include <sstream>
#include <type_traits>

template <typename _Arguments>
class TestCase : public TestCaseInterface {
  public:
    using Arguments = _Arguments;
    static_assert(std::is_base_of_v<Arguments, Arguments>, "Arguments class should derive from Arguments");

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
        bool error = false;
        if (!parseArguments(arguments, commandLineArguments)) {
            return false;
        }

        // Check if all command line arguments were processed (no ignoring)
        if (const auto unprocessedArgs = CommandLineArgument::getUnprocessedArguments(commandLineArguments); !unprocessedArgs.empty()) {
            const auto getKey = +[](const CommandLineArgument *a) { return a->getKey(); };
            std::cerr << CommonHelpMessage::errorIgnoredCommandLineArgs() << joinStrings(", ", unprocessedArgs, getKey) << std::endl;
            error = true;
        }

        // Check if all test case arguments were set (no defaults)
        if (const auto unparsedArgs = arguments.getUnparsedArguments(); !unparsedArgs.empty()) {
            const auto getKey = +[](const Argument *a) { return a->getKey(); };
            std::cerr << CommonHelpMessage::errorUnsetArguments() << joinStrings(", ", unparsedArgs, getKey) << std::endl;
            error = true;
        }

        if (error) {
            return false;
        }

        // Try running with all possible APIs. If some are disabled, e.g. --api=ocl is passed, then the rest will be skipped in run() method
        TestCaseStatistics::printStatisticsHeader(Configuration::get().printType);
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
                      << Configuration::get().iterations << ".\n";
        }
        arguments.iterations = Configuration::get().iterations;
        arguments.noIntelExtensions = Configuration::get().noIntelExtensions;

        // Create statistics object
        const auto testCaseNameWithConfig = getTestCaseNameWithConfig(arguments, Configuration::get().dumpCommandLines);
        TestCaseStatistics statistics{arguments.iterations, Configuration::get().printType};

        // Run test
        const auto testResult = runImpl(statistics, arguments, testCaseNameWithConfig);
        if (testResult == TestResult::Success) {
            FATAL_ERROR_UNLESS(statistics.isFull(), "test did not generate as many values as expected");
            statistics.printStatistics(testCaseNameWithConfig);
        } else {
            const auto &testResultInfo = TestResultHelper::getTestResultInfo(testResult);

            // If test was skipped at the very beginning, it shouldn't have pushed any statistics
            FATAL_ERROR_IF(testResultInfo.wasTestSkipped && !statistics.isEmpty(), "test was skipped but generated some values");

            // Print output line with error info if needed
            const auto printMessage = arguments.isSingleTestMode ? testResultInfo.printInSingleTestMode : testResultInfo.printInAllTestsMode;
            if (printMessage) {
                statistics.printStatisticsString(testCaseNameWithConfig, testResultInfo.stringMessage);
            }
        }
    }

  private:
    TestResult runImpl(Statistics &statistics, const Arguments &arguments, const std::string &testCaseNameWithConfig) const {
        // Get API
        const auto selectedApi = Configuration::get().selectedApi;
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
        const auto &testMap = TestMap::get();
        if (testMap.find(getTestCaseName()) == testMap.end()) {
            printTestMapWarning();
        }

        // Check test filters
        if (auto testFilters = Configuration::get().testFilter.get(); testFilters.size() > 0) {
            const auto testCaseName = getTestCaseName();
            const auto matches = [&](const std::string &testFilter) { return testFilter == testCaseName; };
            const auto requirementMet = std::any_of(testFilters.begin(), testFilters.end(), matches);
            if (!requirementMet) {
                return TestResult::FilteredOut;
            }
        }

        // Check arg filters
        for (const std::string &argFilter : Configuration::get().argFilter.get()) {
            const std::vector<Argument *> &args = arguments.arguments;
            const auto matches = [&](Argument *arg) { return arg->toString() == argFilter; };
            const bool requirementMet = std::any_of(args.begin(), args.end(), matches);
            if (!requirementMet) {
                return TestResult::FilteredOut;
            }
        }

        // Check if test case name with config is not too long
        if (!Configuration::get().dumpCommandLines && !arguments.isSingleTestMode) {
            printTestCaseNameLengthWarning(testCaseNameWithConfig);
        }

        // Noop if required
        if (Configuration::get().noop) {
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
        const static size_t columnWidth = BenchmarkInfo::get().getTestCaseNameColumnWidth();
        static size_t maxWidth = columnWidth;

        const size_t currentWidth = testCaseNameWithConfig.length();
        if (currentWidth > maxWidth) {
            maxWidth = currentWidth;
            std::cerr << "WARNING: current TestCase column width of " << columnWidth << " is too small. Consider changing it to " << maxWidth << ". "
                      << "This is an issue in the benchmark which may cause the output to appear weird, but does not break any functionality.\n";
        }
    }
};
