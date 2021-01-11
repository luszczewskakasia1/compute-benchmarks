#pragma once

#include "framework/test_case/test_result.h"
#include "framework/utility/common_help_message.h"
#include "framework/workload/workload.h"
#include "framework/workload/workload_parameters.h"
#include "framework/workload/workload_statistics.h"

#include <functional>

template <typename _ParametersT>
class Workload {
  public:
    using ParametersT = _ParametersT;
    static_assert(std::is_base_of_v<WorkloadParameters, ParametersT>, "Parameters class should derive from WorkloadArguments");
    using WorkloadImplementation = std::function<TestResult(ParametersT, WorkloadStatistics &)>;
    using ProcessResult = int;

    static inline WorkloadImplementation implementation = {};

    ProcessResult runFromCommandLine(int argc, char **argv) {
        Configuration::loadDefaultConfiguration();

        CommandLineArguments commandLineArguments = {};
        std::string commandLineArgumentsParsingErrors = {};
        if (!CommandLineArgument::parseArguments(argc, argv, commandLineArguments, commandLineArgumentsParsingErrors)) {
            std::cerr << commandLineArgumentsParsingErrors << std::endl;
            return 1;
        }

        ParametersT parameters{};

        if (!parameters.parseArguments(commandLineArguments)) {
            std::cerr << "Error parsing command line" << std::endl;
            return 1;
        }

        bool error = false;

        if (const auto unparsedArgs = parameters.getUnparsedArguments(); !unparsedArgs.empty()) {
            const auto getKey = +[](const TestCaseArgument *a) { return a->getKey(); };
            std::cerr << CommonHelpMessage::errorUnsetArguments() << joinStrings(", ", unparsedArgs, getKey) << std::endl;
            error = true;
        }

        if (const auto unprocessedArgs = CommandLineArgument::getUnprocessedArguments(commandLineArguments); !unprocessedArgs.empty()) {
            const auto getKey = +[](const CommandLineArgument *a) { return a->getKey(); };
            std::cerr << CommonHelpMessage::errorIgnoredCommandLineArgs() << joinStrings(", ", unprocessedArgs, getKey) << std::endl;
            error = true;
        }

        if (error) {
            return toProcessResult(TestResult::InvalidArgs);
        }

        return run(parameters);
    }

    ProcessResult run(const ParametersT &parameters) {
        WorkloadStatistics statistics{};
        TestResult result = runImpl(parameters, statistics);
        if (result == TestResult::Success) {
            statistics.printStatistics();
        }
        return toProcessResult(result);
    }

  private:
    TestResult runImpl(const ParametersT &parameters, WorkloadStatistics &statistics) {
        if (implementation == nullptr) {
            return TestResult::NoImplementation;
        }

        if (!parameters.validateArguments()) {
            return TestResult::InvalidArgs;
        }

        return implementation(parameters, statistics);
    }

    static ProcessResult toProcessResult(TestResult testResult) {
        return static_cast<ProcessResult>(testResult);
    }
};
