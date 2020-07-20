#include "framework/gtest_event_listener.h"
#include "framework/statistics.h"
#include "tests/test_map.h"

#include <gtest/gtest.h>
#include <iostream>
#include <string>

int executeSingleTest(const std::string &testName, int argc, char **argv) {
    const TestMap testMap = getTestMap();
    auto it = testMap.find(testName);
    if (it == testMap.end()) {
        std::cerr << "Unknown test case\n";
        return 1;
    }

    TestCaseInterface *testCase = it->second.get();
    Statistics::printStatisticsHeader();
    if (!testCase->runFromCommandLine(argc, argv)) {
        std::cerr << "Error parsing command line\n";
        return 1;
    }
    return 0;
}

int executeAllTests(int argc, char **argv) {
    std::cout << "executeAllTests\n";
    ::testing::InitGoogleTest(&argc, argv);

    auto &listeners = ::testing::UnitTest::GetInstance()->listeners();
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new CustomEventListener());

    Statistics::printStatisticsHeader();
    return RUN_ALL_TESTS();
}

int printHelp() {
    std::cout << "UllsBenchmark is a set of tests aimed at measuring Ultra Low Latency Submission (ULLS) performance impact. "
                 "It works in two modes described below. Example invocations:\n"
                 "\t.\\ulls_benchmark.exe\n"
                 "\t.\\ulls_benchmark.exe --gtest_filter=*NewResourcesSubmission*\n"
                 "\t.\\ulls_benchmark.exe --test=EmptyKernel --workgroupSize=64 --workgroupCount=30\n"
                 "\t.\\ulls_benchmark.exe --test=EmptyKernel --api=ocl --workgroupSize=64 --workgroupCount=30\n"
                 "\n"
                 "Fist mode is the default and it runs all available benchmarks in many predefined configurations. Underlying test engine "
                 "is googletest, so standard googletest arguments like --gtest_filter can be used, if necessary."
                 "\n\n"
                 "Second mode runs one specific benchmark with custom parameter values. Running benchmarks in this fashion requires "
                 "using --test argument, followed by benchmark-specific parameters. Compute API (ocl or levelzero) can be selected with "
                 "the --api parameter."
                 "\n\n"
                 "Available test cases:"
                 "\n";
    for (const auto &entry : getTestMap()) {
        TestCaseInterface &testCase = *entry.second.get();
        std::cout << testCase.getTestCaseName() << " - " << testCase.getHelp();
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
    if (argc > 1) {
        const std::string firstArgument{argv[1]};

        const std::string singleTestParamName = "--test=";
        if (firstArgument.find(singleTestParamName) == 0) {
            std::string testName = firstArgument.substr(singleTestParamName.size());
            return executeSingleTest(testName, argc, argv);
        }

        if (firstArgument == "-h" || firstArgument == "--help") {
            return printHelp();
        }
    }
    return executeAllTests(argc, argv);
}
