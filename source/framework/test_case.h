#pragma once

#include "framework/api.h"
#include "framework/statistics.h"
#include "framework/string_utils.h"

#include <cassert>
#include <string>
#include <type_traits>

extern int gtestIterations;

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
    int iterations = 1;
};

template <typename Arguments>
class TestCase : public TestCaseInterface {
    static_assert(std::is_base_of_v<TestCaseArguments, Arguments>, "Arguments class should from TestCaseArguments");

  public:
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
        // Run test
        Statistics statistics{arguments.iterations};
        bool testWasRun = false;
        switch (arguments.api) {
        case Api::OpenCL:
            testWasRun = runOcl(arguments, statistics);
            break;
        case Api::L0:
            testWasRun = runL0(arguments, statistics);
            break;
        default:
            std::cerr << "WARNING: unknown API selected. Test was skipped.\n";
            break;
        }
        if (!testWasRun) {
            assert(statistics.isEmpty());
            return;
        }
        assert(statistics.isFull());

        // Output performance results
        const auto apiString = std::string{arguments.api == Api::OpenCL ? "api=ocl" : "api=l0"};
        const auto currentConfig = arguments.getCurrentConfig();
        const auto configWithApi = currentConfig.size() == 0 ? apiString : apiString + "," + currentConfig;
        const auto testCaseNameWithConfig = getTestCaseName() + "(" + configWithApi + ")";
        statistics.printStatistics(testCaseNameWithConfig);
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

        if (key == "--iterations") {
            arguments.iterations = std::atoi(value.c_str());
        }

        return true;
    }

    virtual bool runOcl(const Arguments &arguments, Statistics &statistics) { return false; }
    virtual bool runL0(const Arguments &arguments, Statistics &statistics) { return false; };

  private:
    Arguments arguments = {};
};
