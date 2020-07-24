#pragma once

#include "framework/api.h"
#include "framework/configuration.h"
#include "framework/statistics.h"
#include "framework/string_utils.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>

struct TestCaseInterface {
    virtual bool runFromCommandLine(int argc, char **argv) = 0;
    virtual std::string getHelp() = 0;
    virtual std::string getHelpParameters() = 0;
    virtual std::string getTestCaseName() = 0;
};

struct TestCaseArguments {
    virtual bool parseArgument(const std::string &key, const std::string &value) { return true; }
    virtual bool validateArguments() { return true; }
    virtual std::string getHelp() { return ""; }
    virtual std::string getCurrentConfig() { return ""; }

    Api api = Api::Default;
    int iterations = 0;
};

template <typename Arguments>
class TestCase : public TestCaseInterface {
    static_assert(std::is_base_of_v<TestCaseArguments, Arguments>, "Arguments class should derive from TestCaseArguments");

  public:
    using BenchmarkImplementation = std::function<bool(Arguments, Statistics &)>;
    static inline BenchmarkImplementation implementations[(int)Api::COUNT];

    TestCase() = default;
    TestCase(Arguments arguments) : arguments(arguments) {
    }

    bool runFromCommandLine(int argc, char **argv) override {
        if (!parseArguments(argc, argv)) {
            return false;
        }
        run();
        return true;
    }

    void run() {
        // Set iterations count from global configuration
        if (arguments.iterations != 0) {
            std::cerr << "WARNING: arguments.iterations was not zero. Overriding with value from global configuration - "
                      << ::configuration.iterations << ".\n";
        }
        arguments.iterations = ::configuration.iterations;

        // Get implementation
        const auto apiIndex = static_cast<int>(arguments.api);
        if (apiIndex >= static_cast<int>(Api::COUNT)) {
            std::cerr << "WARNING: unknown API selected. Test was skipped.\n";
            return;
        }
        const auto benchmarkImplementation = implementations[apiIndex];
        if (benchmarkImplementation == nullptr) {
            return;
        }

        // Run test
        Statistics statistics{::configuration.iterations};
        const bool testWasRun = benchmarkImplementation(arguments, statistics);
        if (!testWasRun) {
            assert(statistics.isEmpty());
            return;
        }
        assert(statistics.isFull());

        // Output performance results
        const auto apiString = std::string{arguments.api == Api::OpenCL ? "api=ocl" : "api=l0"};
        const auto currentConfig = arguments.getCurrentConfig();
        const auto configWithApi = currentConfig.size() == 0 ? apiString : apiString + " " + currentConfig;
        const auto testCaseNameWithConfig = getTestCaseName() + "(" + configWithApi + ")";
        statistics.printStatistics(testCaseNameWithConfig, ::configuration.printType);
    }

    std::string getHelpParameters() override {
        return arguments.getHelp();
    }

  protected:
    bool parseArguments(int argc, char **argv) {
        for (int i = 2; i < argc; i++) {
            const auto argument = std::string{argv[i]};
            std::string key, value;
            if (!parseArgumentToKeyValue(argument, key, value)) {
                return false;
            }

            if (!parseArgumentBase(key, value)) {
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

    bool parseArgumentBase(const std::string &key, const std::string &value) {
        if (key == "--api") {
            if (value == "opencl" || value == "ocl") {
                arguments.api = Api::OpenCL;
            } else if (value == "l0" || value == "levelzero") {
                arguments.api = Api::L0;
            } else {
                return false;
            }
        }
        return true;
    }

  private:
    Arguments arguments = {};
};
