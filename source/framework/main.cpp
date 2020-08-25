#include "framework/benchmark_info.h"
#include "framework/configuration.h"
#include "framework/gtest_event_listener.h"
#include "framework/print_device_info.h"
#include "framework/statistics.h"

#include <gtest/gtest.h>
#include <iostream>
#include <string>

int executeSingleTest(const std::string &testName, int argc, char **argv) {
    printDeviceInfo();

    const TestMap testMap = getTestMap();
    auto it = testMap.find(testName);
    if (it == testMap.end()) {
        std::cerr << "Unknown test case\n";
        return 1;
    }

    TestCaseInterface *testCase = it->second.get();
    Statistics::printStatisticsHeader(::configuration.printType, getMeasurmentsUnit());
    if (!testCase->runFromCommandLine(argc, argv)) {
        std::cerr << "Error parsing command line\n";
        return 1;
    }
    return 0;
}

int executeAllTests(int argc, char **argv) {
    printDeviceInfo();

    ::testing::InitGoogleTest(&argc, argv);
    auto &listeners = ::testing::UnitTest::GetInstance()->listeners();
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new CustomEventListener());
    return RUN_ALL_TESTS();
}

int printHelp() {
    const auto filename = getBenchmarkFilename();
    // clang-format off
    std::cout << "UllsBenchmark is a set of tests aimed at measuring Ultra Low Latency Submission (ULLS) performance impact. "
                 "It works in two modes described below. Parameters applicable for both modes:\n"
                 "\t--iterations=X        - select how many times each test will be run\n"
                 "\t--csv                 - dump results in CSV format for easy imports to spreadsheets\n"
                 "\t--verbose             - dump results from all iterations\n"
                 "\t--api                 - select graphics API to use. Possible values: ocl, l0, all\n"
                 "\t--no-intel-extensions - do not run benchmark requiring Intel specific extensions.\n"
                 "\t--oclPlatform         - OpenCL platform index.\n"
                 "\t--oclDevice           - OpenCL device index inside the platform.\n"
                 "\t--l0Driver            - LevelZero driver index.\n"
                 "\t--l0Device            - LevelZero device index inside the driver.\n"
                 "\n"
                 "Example invocations:\n"
                 "\t" << filename << "\n"
                 "\t" << filename << " --api=l0\n"
                 "\t" << filename << " --iterations=100 --csv\n"
                 "\t" << filename << " --gtest_filter=*NewResourcesSubmissionHost*\n"
                 "\t" << filename << " --test=EmptyKernel --workgroupSize=64 --workgroupCount=30\n"
                 "\t" << filename << " --test=EmptyKernel --api=ocl --workgroupSize=64 --workgroupCount=30\n"
                 "\t" << filename << " --test=EmptyKernel --api=ocl --workgroupSize=64 --workgroupCount=30 --iterations=100\n"
                 "\n"
                 "First mode is the default and it runs all available benchmarks in many predefined configurations. Underlying test engine "
                 "is googletest, so standard googletest arguments like --gtest_filter can be used, if necessary.\n"
                 "\n"
                 "Second mode runs one specific benchmark with custom parameter values. Running benchmarks in this fashion requires "
                 "using --test argument, along with benchmark-specific parameters. All parameters have to be specified, there are no "
                 "default values. Available test cases:\n"
                 "\n";
    // clang-format on
    for (const auto &entry : getTestMap()) {
        TestCaseInterface &testCase = *entry.second.get();
        std::cout << '\t' << testCase.getTestCaseName() << " - " << testCase.getHelp();
        const auto helpParameters = testCase.getHelpParameters();
        if (helpParameters.size() != 0) {
            std::cout << " Parameters:\n"
                      << helpParameters;
        }
        std::cout << '\n';
    }
    return 0;
}

int main(int argc, char **argv) {
    if (!parseArgumentsForConfiguration(argc, argv)) {
        std::cerr << "Error parsing command line\n";
        return 1;
    }

    for (int argIndex = 1; argIndex < argc; argIndex++) {
        const std::string argument{argv[argIndex]};
        std::string key, value;
        if (!parseArgumentToKeyValue(argument, key, value)) {
            std::cerr << "Error parsing command line\n";
            return 1;
        }

        if (key == "test") {
            return executeSingleTest(value, argc, argv);
        }
        if (key == "help") {
            return printHelp();
        }
    }

    return executeAllTests(argc, argv);
}
