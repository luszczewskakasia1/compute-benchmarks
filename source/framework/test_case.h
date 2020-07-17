#pragma once

#include "framework/api.h"
#include "framework/statistics.h"

#include <cassert>
#include <string>
#include <type_traits>

struct TestCaseInterface {
    virtual bool runFromCommandLine(int argc, char **argv) = 0;
    virtual std::string getHelp() = 0;
    virtual std::string getTestCaseName() = 0;
};

struct TestCaseArguments {
    virtual bool parseArgument(const std::string &key, const std::string &value) { return true; }
    virtual bool validateArguments() { return true; }
};

template <typename Arguments>
class TestCase : public TestCaseInterface {
    static_assert(std::is_base_of_v<TestCaseArguments, Arguments>, "Arguments class should from TestCaseArguments");

  public:
    TestCase() = default;
    TestCase(Api api, Arguments arguments, int iterations)
        : api(api),
          arguments(arguments),
          iterations(iterations) {}

    bool runFromCommandLine(int argc, char **argv) override {
        if (!parseArguments(argc, argv)) {
            return false;
        }
        run();
        return true;
    }

    void run() {
        Statistics statistics{iterations};
        if (api == Api::OpenCL) {
            runOcl(arguments, statistics, iterations);
        } else {
            runL0(arguments, statistics, iterations);
        }
        assert(statistics.isFull());
        statistics.printStatistics(getTestCaseName() + getTestCaseConfig(arguments));
    }

  protected:
    bool parseArguments(int argc, char **argv) {
        for (int i = 2; i < argc; i++) {
            const auto argument = std::string{argv[i]};
            size_t index = argument.find('=');
            if (argument.find('=', index + 1) != std::string::npos) {
                return false;
            }

            const auto key = argument.substr(0, index);
            const auto value = argument.substr(index + 1);

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
                api = Api::OpenCL;
            } else if (value == "l0" || value == "levelzero") {
                api = Api::L0;
            } else {
                return false;
            }
        }

        if (key == "--iterations") {
            iterations = std::atoi(value.c_str());
        }

        return true;
    }

    virtual std::string getTestCaseConfig(const Arguments &arguments) = 0;
    virtual void runOcl(const Arguments &arguments, Statistics &statistics, int iterations) = 0;
    virtual void runL0(const Arguments &arguments, Statistics &statistics, int iterations) {
        // Dummy Implementation
        for (int i = 0; i < iterations; i++) {
            statistics.pushValue(-1);
        }
    };

  private:
    Api api = Api::OpenCL;
    Arguments arguments = {};
    int iterations = 10;
};
