#pragma once

#include "framework/api.h"
#include "framework/argument.h"
#include "framework/configuration.h"
#include "framework/error.h"
#include "framework/statistics.h"
#include "framework/string_utils.h"

#include <functional>
#include <iostream>
#include <type_traits>

struct TestCaseInterface {
    virtual bool runFromCommandLine(int argc, char **argv) = 0;
    virtual std::string getHelp() const = 0;
    virtual std::string getHelpParameters() const = 0;
    virtual std::string getTestCaseName() const = 0;
};

struct TestCaseArguments {
    virtual bool parseArgument(const std::string &key, const std::string &value) {
        for (auto &argument : arguments) {
            argument->parse(key, value);
        }
        return true;
    }
    virtual bool validateArguments() const {
        for (const auto &argument : arguments) {
            if (!argument->validate()) {
                return false;
            }
        }
        return true;
    }
    virtual std::string getHelp() const {
        std::ostringstream result;
        for (const auto &argument : arguments) {
            result << "\t\t" << argument->getHelp() << '\n';
        }
        return result.str();
    }
    virtual std::string getCurrentConfig() const {
        std::ostringstream result;
        for (auto i = 0; i < arguments.size(); i++) {
            result << arguments[i]->toString();
            if (i != arguments.size() - 1) {
                result << " ";
            }
        }
        return result.str();
    }

    std::vector<TestCaseArgument *> arguments;
    Api api = Api::Default;
    int iterations = 0;
};

enum class TestResult {
    Success,
    Error,
    DriverFunctionNotFound,
    KernelNotFound,
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
