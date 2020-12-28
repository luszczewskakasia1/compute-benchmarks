#include "framework/benchmark_info.h"
#include "framework/configuration.h"
#include "framework/gtest_event_listener.h"
#include "framework/print_device_info.h"
#include "framework/utility/statistics.h"

#include <gtest/gtest.h>
#include <iostream>

int printVersion(bool enableWarning, const char *prefix = "") {
    const std::string version = BENCHMARK_VERSION;
    if (!version.empty()) {
        std::cout << prefix << BENCHMARK_VERSION << std::endl;
        return 0;
    }

    if (enableWarning) {
        std::cerr << "Unknown version. Run CMake with \"-D INCLUDE_VERSION=ON\" to include it in the binary." << std::endl;
        return 1;
    }

    return 0;
}

int executeSingleTest(const std::string &testName, CommandLineArguments &commandLineArguments) {
    printDeviceInfo();
    printVersion(false, "Benchmark version: ");

    const TestMap &testMap = getTestMap();
    auto it = testMap.find(testName);
    if (it == testMap.end()) {
        std::cerr << "Unknown test case\n";
        return 1;
    }

    TestCaseInterface *testCase = it->second.get();
    if (!testCase->runFromCommandLine(commandLineArguments)) {
        std::cerr << "Error parsing command line\n";
        return 1;
    }
    return 0;
}

int executeAllTests(int argc, char **argv) {
    printDeviceInfo();
    printVersion(false, "Benchmark version: ");

    ::testing::InitGoogleTest(&argc, argv);
    auto &listeners = ::testing::UnitTest::GetInstance()->listeners();
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new CustomEventListener());
    return RUN_ALL_TESTS();
}

int printHelp() {
    const auto filename = getBenchmarkFilename();
    // clang-format off
    std::cout << getBenchmarkDescription() << "\n"
                 "\n"
                 "The benchmark works in two modes - all-tests mode and single-test mode. They are further described below. "
                 "Global parameters applicable for both modes:\n"
                 << ::configuration.getHelp(1u) << "\n"
                 "\n"
                 "First mode is the default and it runs all available benchmarks in many predefined configurations. Underlying test engine "
                 "is googletest, so standard googletest arguments like --gtest_filter can be used, if necessary.\n"
                 "\n"
                 "Second mode runs one specific benchmark with custom parameter values. Running benchmarks in this fashion requires "
                 "using --test argument, along with benchmark-specific parameters. All parameters have to be specified, there are no "
                 "default values.\n"
                 "\n"
                 "Example invocations:\n"
                 "\t" << filename << "                                                runs all possible tests\n"
                 "\t" << filename << " --api=ocl                                      runs all possible OpenCL tests\n"
                 "\t" << filename << " --iterations=100 --csv                         runs all possible tests with 100 iterations and dumps results as CSV\n"
                 "\t" << filename << " --gtest_filter=<regex>                         runs all tests matching a regular expression\n"
                 "\t" << filename << " --gtest_filter=*TestName*                      runs a test named \"TestName\" in all predefined configurations\n"
                 "\t" << filename << " --test=TestName --someParam=1 --otherParam=30  runs a test named \"TestName\" with specified parameters\n"
                 "\n"
                "All available test cases with their parameters:\n";
    // clang-format on
    for (const auto &entry : getTestMap()) {
        TestCaseInterface &testCase = *entry.second.get();
        std::cout << '\t' << testCase.getTestCaseName() << " - " << testCase.getHelp();
        const auto helpParameters = testCase.getHelpParameters();
        if (helpParameters.size() != 0) {
            std::cout << " Parameters:\n"
                      << helpParameters;
        } else {
            std::cout << '\n';
        }
        std::cout << '\n';
    }
    return 0;
}

int main(int argc, char **argv) {
    CommandLineArguments commandLineArguments = {};
    std::string commandLineArgumentsParsingErrors = {};
    if (!CommandLineArgument::parseArguments(argc, argv, commandLineArguments, commandLineArgumentsParsingErrors)) {
        std::cerr << commandLineArgumentsParsingErrors << std::endl;
        return 1;
    }

    if (!parseArgumentsForConfiguration(commandLineArguments)) {
        std::cerr << "Error parsing command line\n";
        return 1;
    }

    for (auto &argument : commandLineArguments) {
        if (argument.isKeyEqualTo("test")) {
            argument.markAsProcessed();
            return executeSingleTest(argument.getValue(), commandLineArguments);
        }
        if (argument.isKeyEqualTo("help")) {
            return printHelp();
        }
        if (argument.isKeyEqualTo("version")) {
            return printVersion(true);
        }
    }

    return executeAllTests(argc, argv);
}
